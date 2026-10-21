/*
 * A285110  a(n) = A001222(A285323(n)).
 * https://oeis.org/A285110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0. For n > 0, let pos1 be the 0‑based position of the
 *            least significant 1‑bit of n, and pos2 the position of the
 *            second least significant 1‑bit. Then a(n) = 2 if n is a power of
 *            two or prime(1+pos2) > prime(1+pos1)^2 a(n) = 1 otherwise.
 *            (Prime indices are 1‑based: prime(1)=2, prime(2)=3, …)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   trailing_zero_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/trailing_zero_count.h"

/* First 70 primes (enough for positions up to 63).  Index 0 -> prime(1). */
static const seq_t primes[] =
{
    /*  1*/ 2,    /*  2*/ 3,    /*  3*/ 5,    /*  4*/ 7,    /*  5*/ 11,
    /*  6*/ 13,   /*  7*/ 17,   /*  8*/ 19,   /*  9*/ 23,   /* 10*/ 29,
    /* 11*/ 31,   /* 12*/ 37,   /* 13*/ 41,   /* 14*/ 43,   /* 15*/ 47,
    /* 16*/ 53,   /* 17*/ 59,   /* 18*/ 61,   /* 19*/ 67,   /* 20*/ 71,
    /* 21*/ 73,   /* 22*/ 79,   /* 23*/ 83,   /* 24*/ 89,   /* 25*/ 97,
    /* 26*/ 101,  /* 27*/ 103,  /* 28*/ 107,  /* 29*/ 109,  /* 30*/ 113,
    /* 31*/ 127,  /* 32*/ 131,  /* 33*/ 137,  /* 34*/ 139,  /* 35*/ 149,
    /* 36*/ 151,  /* 37*/ 157,  /* 38*/ 163,  /* 39*/ 167,  /* 40*/ 173,
    /* 41*/ 179,  /* 42*/ 181,  /* 43*/ 191,  /* 44*/ 193,  /* 45*/ 197,
    /* 46*/ 199,  /* 47*/ 211,  /* 48*/ 223,  /* 49*/ 227,  /* 50*/ 229,
    /* 51*/ 233,  /* 52*/ 239,  /* 53*/ 241,  /* 54*/ 251,  /* 55*/ 257,
    /* 56*/ 263,  /* 57*/ 269,  /* 58*/ 271,  /* 59*/ 277,  /* 60*/ 281,
    /* 61*/ 283,  /* 62*/ 293,  /* 63*/ 307,  /* 64*/ 311,  /* 65*/ 313,
    /* 66*/ 317,  /* 67*/ 331,  /* 68*/ 337,  /* 69*/ 347,  /* 70*/ 349
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 0 */

    if ((n & (n - 1)) == 0) return 2;   /* power of two */

    unsigned pos1 = trailing_zero_count(n);
    unsigned pos2 = pos1 + 1 + trailing_zero_count(n >> (pos1 + 1));

    /* Retrieve primes using 0‑based indices (prime(1+pos) -> primes[pos]) */
    seq_t p1 = primes[pos2];
    seq_t p2 = primes[pos1];

    if (p1 > p2 * p2) return 2;
    return 1;
}
