/*
 * A360110  Nonmultiples of 4 whose arithmetic derivative is a multiple of 4.
 * https://oeis.org/A360110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th positive integer that is not divisible by 4 and
 *            whose arithmetic derivative is a multiple of 4. (computed by
 *            inspecting each integer and memoising results)
 * Range:     u8 n <= 26, u16 n <= 8119, u32 n <= 9863, u64 n <= 9863,
 *            u128 n <= 9863, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache of computed terms */
static seq_t *terms      = NULL;
static size_t terms_cnt  = 0;

/* last integer that has been tested for the property */
static uint64_t last_tested = 0;

/* add a new term into the cache */
static void add_term(uint64_t val)
{
    seq_t *tmp = (seq_t *)realloc(terms, (terms_cnt + 1) * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    terms = tmp;
    terms[terms_cnt++] = (seq_t)val;
}

/* return true if m satisfies the property:
   m is not a multiple of 4 and m' (arithmetic derivative of m) is a multiple of 4 */
static bool has_property(uint64_t m)
{
    /* exclude multiples of 4 */
    if ((m & 3ULL) == 0) return false;
    if (m == 1) return true;                 /* 1' = 0, divisible by 4 */

    uint64_t n   = m;
    uint32_t sum = 0;                         /* sum of terms modulo 4 */

    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        uint32_t e = 0;
        while (!(n & 1ULL)) { n >>= 1; e++; }
        uint32_t part = ((m / 2ULL) & 3U);
        uint32_t term = (part * (e & 3U)) & 3U;
        sum = (sum + term) & 3U;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if ((n % p) == 0) {
            uint32_t e = 0;
            while (n % p == 0) { n /= p; e++; }
            uint32_t part = ((m / p) & 3U);
            uint32_t term = (part * (e & 3U)) & 3U;
            sum = (sum + term) & 3U;
        }
    }

    /* remaining prime factor, if any */
    if (n > 1) {
        uint32_t e = 1;
        uint32_t part = ((m / n) & 3U);
        uint32_t term = (part * (e & 3U)) & 3U;
        sum = (sum + term) & 3U;
    }

    return sum == 0;
}

/* public function required by OEIS harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; value for n==0 is irrelevant */

    /* ensure we have at least n terms precomputed */
    while (terms_cnt < (size_t)n) {
        ++last_tested;
        if (has_property(last_tested))
            add_term(last_tested);
    }

    return terms[n - 1];
}
