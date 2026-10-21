/*
 * A251107  Number of (n+1) X (2+1) 0..2 arrays with no 2 X 2 subblock having
 *          the maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A251107: Number of (n+1) x (2+1) 0..2 arrays with no subblock
 * having the maximum diagonal greater than the absolute difference of its
 * antidiagonal.  Recurrence of order 14:
 *
 *   a(n) = 11*a(n-1) - 53*a(n-2) + 169*a(n-3) - 400*a(n-4) + 717*a(n-5)
 *          - 999*a(n-6) + 1063*a(n-7) - 860*a(n-8) + 543*a(n-9)
 *          - 268*a(n-10) + 115*a(n-11) - 40*a(n-12) + 9*a(n-13) - a(n-14)
 *
 * Initial values (n = 1..14):
 *   147, 810, 3616, 15281, 67518, 304870, 1369052, 6118942,
 *   27356256, 122402144, 547700666, 2450461705, 10963429165,
 *   49051483677
 *
 * The function recomputes the sequence from scratch for each call; it
 * uses only a fixed-size buffer of 14 terms and therefore works for any
 * n >= 1 within the time limits.  It is compatible with C11 and C++17.
 */

#include <stddef.h>
#include <stdint.h>

/* The types seq_t and idx_t are provided by the build system. */
extern seq_t a(idx_t n);

/* ---------- Implementation ---------- */

static const seq_t init_vals[14] = {
    (seq_t)147ULL,          /* a(1) */
    (seq_t)810ULL,          /* a(2) */
    (seq_t)3616ULL,         /* a(3) */
    (seq_t)15281ULL,        /* a(4) */
    (seq_t)67518ULL,        /* a(5) */
    (seq_t)304870ULL,       /* a(6) */
    (seq_t)1369052ULL,      /* a(7) */
    (seq_t)6118942ULL,      /* a(8) */
    (seq_t)27356256ULL,     /* a(9) */
    (seq_t)122402144ULL,    /* a(10) */
    (seq_t)547700666ULL,    /* a(11) */
    (seq_t)2450461705ULL,   /* a(12) */
    (seq_t)10963429165ULL,  /* a(13) */
    (seq_t)49051483677ULL   /* a(14) */
};

seq_t a(idx_t n)
{
    /* Return 0 for indices below the sequence offset. */
    if (n < 1)
        return (seq_t)0;

    /* For n <= 14, return the precomputed initial value. */
    if (n <= 14)
        return init_vals[n - 1];

    /* Circular buffer holding the last 14 computed terms.
       prev[0] = a(k-14), prev[13] = a(k-1) for the current step k. */
    seq_t prev[14];
    for (size_t i = 0; i < 14; ++i)
        prev[i] = init_vals[i];

    seq_t next;
    for (idx_t k = 15; k <= n; ++k)
    {
        next = (seq_t)0;
        next += (seq_t)11   * prev[13]; /* a(k-1) */
        next -= (seq_t)53   * prev[12]; /* a(k-2) */
        next += (seq_t)169  * prev[11]; /* a(k-3) */
        next -= (seq_t)400  * prev[10]; /* a(k-4) */
        next += (seq_t)717  * prev[9];  /* a(k-5) */
        next -= (seq_t)999  * prev[8];  /* a(k-6) */
        next += (seq_t)1063 * prev[7];  /* a(k-7) */
        next -= (seq_t)860  * prev[6];  /* a(k-8) */
        next += (seq_t)543  * prev[5];  /* a(k-9) */
        next -= (seq_t)268  * prev[4];  /* a(k-10) */
        next += (seq_t)115  * prev[3];  /* a(k-11) */
        next -= (seq_t)40   * prev[2];  /* a(k-12) */
        next += (seq_t)9    * prev[1];  /* a(k-13) */
        next -=           prev[0];     /* a(k-14) */

        /* Shift the buffer: drop a(k-14), shift others left, insert new term. */
        for (size_t i = 0; i < 13; ++i)
            prev[i] = prev[i + 1];
        prev[13] = next;
    }

    /* After the loop, prev[13] holds a(n). */
    return prev[13];
}
