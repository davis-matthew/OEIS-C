/*
 * A351225  a(n) = A276086(n) - n, where A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A351225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A276086(n) - n where A276086(n) is the primorial‑base
 *            exponential value of n
 * Range:     i8 n <= 14, i16 n <= 57, i32 n <= 412, i64 n <= 2306,
 *            i128 n <= 9943, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include minimal standard headers */
#include <stdint.h>
#include <stddef.h>

/* ---------- constants ---------- */
static const uint32_t PRIMES[20] = {
    2,   3,   5,   7,  11,  13,  17,  19,
   23,  29,  31,  37,  41,  43,  47,  53,
   59,  61,  67,  71
};
#define MAX_PRIMES (sizeof(PRIMES)/sizeof(PRIMES[0]))

/* ---------- helper function ---------- */
seq_t a(idx_t n)
{
    /* Array of primorial weights (product of first i primes). */
    static unsigned __int128 wts[MAX_PRIMES];
    size_t used = 0;

    /* Build weights until the largest one is > n or we run out of primes. */
    unsigned __int128 cur = 1;
    while (cur <= (unsigned __int128)n && used < MAX_PRIMES) {
        wts[used] = cur;
        cur *= (unsigned __int128)PRIMES[used];
        ++used;
    }

    /* Compute the primorial‑base digit representation of n. */
    idx_t remainder = n;          /* remaining value to represent */
    seq_t prod = 1;                /* product of p_i^d_i */

    /* Process weights from most significant to least. */
    for (long long j = (long long)used - 1; j >= 0; --j) {
        uint64_t weight = (uint64_t)wts[j];
        uint64_t digit  = remainder / weight;
        uint64_t lim    = (uint64_t)PRIMES[j] - 1;          /* d_j < p_j */

        if (digit > lim) digit = lim;

        /* Multiply prod by p_j^digit. */
        uint32_t prime = PRIMES[j];
        for (uint32_t k = 0; k < digit; ++k)
            prod *= (seq_t)prime;

        remainder -= digit * weight;
    }

    return prod - (seq_t)n;
}
