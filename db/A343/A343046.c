/*
 * A343046  Array T(n, k), n, k >= 0, read by antidiagonals; lunar
 *          multiplication table for the primorial base.
 * https://oeis.org/A343046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) is the n-th entry read by antidiagonals of the lunar
 *            multiplication table T over primorial base, where T is the lunar
 *            product in primorial base. This implementation computes a(n) by
 *            determining the pair (i,j) of indices from the antidiagonal
 *            order and then evaluating the lunar product of i a...
 * Range:     u8 n <= 262, u16 n <= 9081, u32 n <= 9081, u64 n <= 9081,
 *            u128 n <= 9081, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* First 40 primes – more than enough for numbers up to 10⁴. */
static const unsigned int PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173
};
#define NUM_PRIMES (sizeof(PRIMES)/sizeof(PRIMES[0]))

/* Convert idx_t n to its primorial‑base digits (least significant first). */
static void get_digits(idx_t n, uint8_t *buf, size_t *len)
{
    size_t idx = 0;
    uint64_t value = (uint64_t)n;
    for (size_t p = 0; p < NUM_PRIMES && value > 0; ++p) {
        uint8_t prime = PRIMES[p];
        buf[idx++] = value % prime;
        value /= prime;
    }
    *len = idx;
}

/* Compute the lunar product of a and b in primorial base. */
static seq_t lunar_product(idx_t a, idx_t b)
{
    uint8_t da[64], db[64];
    size_t la, lb;
    get_digits(a, da, &la);
    get_digits(b, db, &lb);

    size_t reslen = la + lb;
    uint8_t res[128] = {0};

    for (size_t i = 0; i < la; ++i) {
        for (size_t j = 0; j < lb; ++j) {
            uint8_t v = da[i] < db[j] ? da[i] : db[j];
            if (v > res[i + j]) res[i + j] = v;
        }
    }

    seq_t value = 0;
    seq_t weight = 1;
    for (size_t k = 0; k < reslen; ++k) {
        if (res[k]) value += (seq_t)res[k] * weight;
        /* Grow the weight for next digit. */
        if (k + 1 < reslen) weight *= (seq_t)PRIMES[k];
    }
    return value;
}

/* Public function as requested. */
seq_t a(idx_t n)
{
    /* Determine the pair (i,j) from the antidiagonal ordering. */
    idx_t d = 0;
    idx_t cum = 0;
    while (cum + (d + 1) <= n) {
        cum += d + 1;
        ++d;
    }
    idx_t offset = n - cum;
    idx_t i = offset;
    idx_t j = d - offset;

    return lunar_product(i, j);
}
