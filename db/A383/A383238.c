/*
 * A383238  A sequence constructed by greedily sampling the Poisson
 *          distribution for parameter value 1, 1/(e*(i-1)!) to minimize
 *          discrepancy selecting the smallest value in case of ties.
 * https://oeis.org/A383238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th element of the greedy Poisson sampling sequence
 *            (OEIS A383238). It is computed by simulating the algorithm that
 *            for each n chooses the k that maximises n*p(k) - c(k), where
 *            p(k) = 1/(e*(k-1)!) and c(k) is the count of occurrences of k so
 *            far. The smallest k breaks ties. This impl...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* constants for a high‑precision rational approximation of e:
   e ≈ 2718281828459045235 / 1000000000000000000   */
static const uint64_t E_NUM = 2718281828459045235ULL;
static const uint64_t E_DEN = 1000000000000000000ULL;

/* pre‑computed factorials up to 14! (the largest needed for n ≤ 10 000) */
static const uint64_t FACT[16] = {
    1ULL,                    /* 0! */
    1ULL,                    /* 1! */
    2ULL,                    /* 2! */
    6ULL,                    /* 3! */
    24ULL,                   /* 4! */
    120ULL,                  /* 5! */
    720ULL,                  /* 6! */
    5040ULL,                 /* 7! */
    40320ULL,                /* 8! */
    362880ULL,               /* 9! */
    3628800ULL,              /*10! */
    39916800ULL,             /*11! */
    479001600ULL,            /*12! */
    6227020800ULL,           /*13! */
    87178291200ULL,          /*14! */
    1307674368000ULL         /*15! (not used but kept for completeness) */
};

/* maximum k considered – 15 is sufficient for n ≤ 10 000 as
   k>15 never reaches an expected count ≥ 1. */
#define K_MAX 15

seq_t a(idx_t n)
{
    /* counts of each value seen so far */
    unsigned int c[K_MAX + 1] = {0};

    /* L = (K_MAX-1)! – used to make the division by factorial exact */
    const uint64_t L = FACT[K_MAX - 1];    /* 14! = 87178291200 */

    unsigned int bestk = 1;
    __int128 bestD = 0;
    int first = 1;      /* flag to initialise bestD */

    for (idx_t i = 1; i <= n; ++i) {
        uint64_t ui = (uint64_t)i;  /* safe cast – i ≤ 10 000 */
        unsigned int chosen = 1;    /* default */
        __int128 bestScore = 0;

        for (int k = 1; k <= K_MAX; ++k) {
            /* multiplier = L / (k-1)!  (an integer) */
            uint64_t mult = L / FACT[k - 1];

            /* term1 = E_DEN * i * mult
               term2 = E_NUM * L * c[k] */
            __int128 term1 = (__int128)E_DEN * (__int128)ui * (__int128)mult;
            __int128 term2 = (__int128)E_NUM * (__int128)L * (__int128)c[k];
            __int128 D = term1 - term2;   /* signed comparison */

            if (first || D > bestScore) {
                bestScore = D;
                chosen = k;
                first = 0;
            }
        }

        /* record the chosen value */
        c[chosen] += 1;
        if (i == n)
            return (seq_t)chosen;
    }

    /* unreachable – but required to satisfy compiler */
    return (seq_t)1;
}
