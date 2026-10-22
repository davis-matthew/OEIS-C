/*
 * A365144  Numbers having each digit once and whose 4th power has each digit
 *          four times.
 * https://oeis.org/A365144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A365144 – numbers with all ten digits once,
   whose fourth power contains each digit exactly four times. */

/* ---------- Auxiliary big‐integer helpers for the 4th power ---------- */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Simple 160‑bit unsigned integer, enough for up to 40 decimal digits */
typedef struct {
    uint32_t d[5];          /* Least–significant word first */
} big_uint;

/* Zero a big_uint */
static inline void big_uint_zero(big_uint *x) { for (int i = 0; i < 5; ++i) x->d[i] = 0; }

/* Initialise from a 64‑bit integer */
static inline void big_uint_from_u64(big_uint *x, uint64_t v) {
    x->d[0] = (uint32_t)v;
    x->d[1] = (uint32_t)(v >> 32);
    x->d[2] = x->d[3] = x->d[4] = 0;
}

/* Multiply two 5‑word numbers, keeping only the lower 5 words of the product */
static void big_uint_mul_into(big_uint *res, const big_uint *a, const big_uint *b) {
    uint64_t tmp[10] = {0};
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            tmp[i + j] += (uint64_t)a->d[i] * b->d[j];
    uint64_t carry = 0;
    for (int k = 0; k < 5; ++k) {
        uint64_t val = tmp[k] + carry;
        res->d[k] = (uint32_t)(val & 0xffffffffU);
        carry = val >> 32;
    }
    /* higher words (tmp[5]..tmp[9]) are discarded – the result fits in 5 words */
}

/* Division by 10 – modifies *x, returns the remainder */
static unsigned char big_uint_div_by_10_quo_rem(big_uint *x) {
    uint64_t rem = 0;
    for (int i = 4; i >= 0; --i) {
        uint64_t cur = (rem << 32) | x->d[i];
        uint64_t q   = cur / 10;
        rem          = cur % 10;
        x->d[i]      = (uint32_t)q;
    }
    return (unsigned char)rem;
}

/* Test for zero */
static inline bool big_uint_is_zero(const big_uint *x) {
    return x->d[0] == 0 && x->d[1] == 0 && x->d[2] == 0 &&
           x->d[3] == 0 && x->d[4] == 0;
}

/* ---------- Next permutation in place (unsinged char array) ---------- */
static bool next_permutation(unsigned char *a, size_t n) {
    if (n < 2) return false;
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (a[j] <= a[i]) --j;
    unsigned char tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    size_t lo = i + 1, hi = n - 1;
    while (lo < hi) {
        tmp = a[lo];
        a[lo] = a[hi];
        a[hi] = tmp;
        ++lo;
        --hi;
    }
    return true;
}

/* ---------- Sequence memoisation ---------- */
#define CACHE_MAX 20000                /* ample for the expected number of terms */
static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;
static bool cache_built = false;

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Build cache on first call */
    if (!cache_built) {
        unsigned char perm[10];
        for (int i = 0; i < 10; ++i) perm[i] = (unsigned char)i;

        do {
            if (perm[0] == 0)          /* no leading zero – number must be 10 digits */
                continue;

            /* Compute the decimal number and its 4th power */
            uint64_t num = 0;
            for (int i = 0; i < 10; ++i) num = num * 10ULL + (uint64_t)perm[i];

            big_uint num_big, pow, temp;
            big_uint_zero(&num_big);
            big_uint_from_u64(&num_big, num);

            /* Start with pow = num */
            big_uint_zero(&pow);
            pow.d[0] = num_big.d[0];
            pow.d[1] = num_big.d[1];
            /* Multiply three more times to get n^4 */
            for (int mul = 0; mul < 3; ++mul) {
                big_uint_mul_into(&temp, &pow, &num_big);
                pow = temp;
            }

            /* Count decimal digits of the power */
            unsigned char cnt[10] = {0};
            big_uint tmp = pow;          /* make a mutable copy */
            while (!big_uint_is_zero(&tmp)) {
                unsigned char r = big_uint_div_by_10_quo_rem(&tmp);
                ++cnt[r];
            }

            /* Test the property: each digit occurs exactly four times */
            bool ok = true;
            for (int d = 0; d < 10; ++d) {
                if (cnt[d] != 4) { ok = false; break; }
            }

            if (ok) {
                /* Store the 10‑digit number as seq_t */
                seq_t val = 0;
                for (int i = 0; i < 10; ++i)
                    val = val * 10 + perm[i];
                if (cache_len < CACHE_MAX)
                    cache[cache_len++] = val;
            }
        } while (next_permutation(perm, 10));

        cache_built = true;
    }

    if (n == 0) return 0;
    if ((size_t)(n - 1) < cache_len)
        return cache[n - 1];
    return 0;     /* n beyond which we have not found terms */
}
