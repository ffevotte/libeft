#!/usr/bin/python -u
# -*- coding: utf-8 -*-

# This file is part of LibEFT, an error-free transformations library
#
# Copyright (C) 2017
#   F. Févotte     <francois.fevotte@edf.fr>
#   B. Lathuilière <bruno.lathuiliere@edf.fr>
#   EDF SA
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307, USA.
#
# The GNU General Public License is contained in the file COPYING.

import math
from numpy import linspace
from numpy.random import shuffle
from fractions import Fraction
from random import random


def dot (x, y, f=lambda x: x):
    """Dot product between x and y.

    If argument f is given, it is a function applied to each coefficient of each
    vector before it is used in computations.

    In particular, f = Fraction allows computing an accurate result using
    rational numbers.
    """
    acc = f(0)
    for i in xrange(len(x)):
        acc += f(x[i]) * f(y[i])
    return acc

def genDot (n, c):
    """Generate two vectors whose dot product is ill-conditioned

    Arguments:
      n -- vectors size
      c -- target condition number

    Results:
      x, y -- vectors of size n
      d    -- accurate dot product, rounded to nearest
      C    -- actual condition number of the dot product
    """

    # Initialization
    x = [0] * n
    y = [0] * n

    # First half of the vectors:
    #   random numbers within a large exponent range
    n2 = int(n / 2)
    b = math.log(c) / math.log(2)

    e = [random() * b/2 for i in xrange(n2)]
    e[0]    = b/2 + 1           # Make sure exponents b/2
    e[n2-1] = 0                 # and 0 actually occur
    for i in xrange(n2):
        x[i] = (2*random()-1) * 2**(e[i])
        y[i] = (2*random()-1) * 2**(e[i])


    # Second half of the vectors such that
    #   (*) log2( dot (x[1:i], y[1:i]) ) decreases from b/2 to 0
    e = linspace (b/2, 0, n-n2)
    for i in xrange(n-n2):
        # Random x[i]
        cx = (2*random()-1) * 2**(e[i])
        x[i+n2] = cx

        # y[i] chosen according to (*)
        cy = (2*random()-1) * 2**(e[i])
        y[i+n2] = (cy - float (dot (x, y, Fraction))) / cx


    # Random permutation of x and y
    perm = range(n); shuffle (perm)
    X = [0] * n
    Y = [0] * n
    for i in xrange(n):
        X[i] = x[perm[i]]
        Y[i] = y[perm[i]]


    # Dot product, rounded to nearest
    d = float (dot (X, Y, Fraction))

    # Actual condition number
    C = 2 * dot (X, Y, abs) / abs(d)

    return [X,Y,d,C]




# Helper functions for standalone Test

def split (a):
    factor = 2**27 + 1
    c = factor * a
    x = c - (c - a)
    y = a - x
    return (x, y)

def twoProd (a, b):
    x = a*b
    a1, a2 = split (a)
    b1, b2 = split (b)
    y = a2*b2 - (((x-a1*b1) - a2*b1) - a1*b2)
    return (x, y)

def twoSum (a, b):
    x = a + b
    if abs(b) > abs(a):
        (a,b) = (b,a)
    z = x - a
    y = b - z
    return (x, y)

def dot1 (x,y):
    acc = 0
    accErr = 0
    for i in xrange(len(x)):
        acc, e = twoSum (acc, x[i] * y[i])
        accErr += e
    return acc + accErr

def dot2 (x,y):
    acc = 0
    accErr = 0
    for i in xrange(len(x)):
        p, e = twoProd (x[i], y[i])
        acc += p
        accErr += e
    return acc + accErr

def dot3 (x,y):
    acc = 0
    accErr = 0
    for i in xrange(len(x)):
        p, ep = twoProd (x[i], y[i])
        acc, es = twoSum (acc, p)
        accErr += ep + es
    return acc + accErr

def err (res, ref):
    return min(1, max(1e-16, abs((res-ref)/ref)))


def send (cmd, x, y):
    p = subprocess.Popen(cmd,
                         stdin=subprocess.PIPE,
                         stdout=subprocess.PIPE)
    p.stdin.write("%d\n" % len(x))
    for i in xrange(len(x)):
        p.stdin.write ("%.18e %.18e\n" % (x[i], y[i]))

    return float(p.stdout.read())

def output (f,x):
    out = "%.5e " % x
    print out,
    f.write(out)

if __name__ == "__main__":
    import sys
    import subprocess

    n = 100
    c1 = 1
    c2 = 1e35
    step = 2

    try:
        n = int(sys.argv[1])
        c1 = float(sys.argv[2])
        c2 = float(sys.argv[3])
        step = float(sys.argv[4])
    except:
        pass

    with open ("qual.dat", "w") as f:
        c = c1
        while c < 1e33:
            (x,y,d,C) = genDot (n, c)
            output (f, C)

            r = dot(x,y)
            output (f, err(r,d))

            for test in ["./testCxx", "./testC", "./testF"]:
                for version in ["std", "comp"]:
                    r = send ([test, "dot", version], x, y)
                    output (f, err(r,d))

            print "\n",
            f.write ("\n")

            c *= step

    subprocess.call (["gnuplot", "qual.gp"])
