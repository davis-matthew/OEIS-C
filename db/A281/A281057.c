/*
 * A281057  Number of 2 X n 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) - 2*a(n-3) + 10*a(n-4) - 2*a(n-5) -
 *            5*a(n-6) + a(n-7) + a(n-8) (n > 14) Initial values: a(1)=0,
 *            a(2)=1, a(3)=6, a(4)=34, a(5)=104, a(6)=251, a(7)=535,
 *            a(8)=1076, a(9)=2090, a(10)=3956, a(11)=7353, a(12)=13474,
 *            a(13)=24417, a(14)=43846
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 35, u64 n <= 78, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values (index 1..14).  Element 0 is unused. */
    static const seq_t base[15] = {
        0ULL,          /* base[0] unused */
        0ULL,          /* a(1) */
        1ULL,          /* a(2) */
        6ULL,          /* a(3) */
        34ULL,         /* a(4) */
        104ULL,        /* a(5) */
        251ULL,        /* a(6) */
        535ULL,        /* a(7) */
        1076ULL,       /* a(8) */
        2090ULL,       /* a(9) */
        3956ULL,       /* a(10) */
        7353ULL,       /* a(11) */
        13474ULL,      /* a(12) */
        24417ULL,      /* a(13) */
        43846ULL       /* a(14) */
    };

    if (n <= 14) {
        return base[n];
    }

    /* Allocate array for terms 1..n inclusive.  seq[0] is unused. */
    size_t size = (size_t)(n + 1);
    seq_t *seq = (seq_t *)malloc(size * sizeof(seq_t));
    if (!seq) {
        /* In a real library we would handle allocation failure; */
        /* here we simply return 0 as a fallback. */
        return 0;
    }

    /* Initialize base values. */
    for (idx_t i = 1; i <= 14; ++i) {
        seq[i] = base[i];
    }

    /* Compute terms a(15) .. a(n) using the recurrence. */
    for (idx_t k = 15; k <= n; ++k) {
        seq_t pos = 0;
        pos += (seq_t)5 * seq[k - 1];
        pos += (seq_t)10 * seq[k - 4];
        pos += seq[k - 7];
        pos += seq[k - 8];

        seq_t neg = 0;
        neg += (seq_t)7 * seq[k - 2];
        neg += (seq_t)2 * seq[k - 3];
        neg += (seq_t)2 * seq[k - 5];
        neg += (seq_t)5 * seq[k - 6];

        /* For the combinatorial sequence we know that pos >= neg, so
           unsigned subtraction yields the correct result. */
        seq[k] = pos - neg;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
