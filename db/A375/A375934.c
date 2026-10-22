/*
 * A375934  Numbers whose prime factorization has a second-largest exponent
 *          that equals 1.
 * https://oeis.org/A375934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose prime factorization has a second‑largest exponent that equals 1.
   The code generates the sequence by scanning the integers in increasing order
   and keeping those that satisfy the condition:
      – at least two distinct prime factors,
      – at least one prime with exponent ≥2,
      – all exponents are either 1 or equal to the maximal exponent.
*/
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Helper: ensure the dynamic cache can hold “need” elements         */
/* ------------------------------------------------------------------ */
static seq_t *CACHE = NULL;
static size_t CACHE_CAP = 0;
static idx_t CACHE_CNT = 0;
static uint64_t NEXT_VAL = 12;                 /* first value to test */

/* Grow the cache array if required. */
static void ensure_cache_cap(size_t need)
{
    if (CACHE_CAP >= need) return;
    size_t new_cap = CACHE_CAP ? CACHE_CAP << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    CACHE = tmp;
    CACHE_CAP = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Check whether a 64‑bit integer satisfies the sequence condition.  */
/* ------------------------------------------------------------------ */
static bool is_valid(uint64_t x)
{
    if (x < 12) return false;                /* all valid numbers are ≥12 */

    uint64_t t = x;
    size_t distinct_primes = 0;
    size_t count_lt_one_exp = 0;             /* number of primes with exponent 1 */
    size_t count_gt_one_exp = 0;             /* number of primes with exponent >1 */

    uint64_t max_exp = 0;                    /* largest exponent seen     */
    uint64_t expected_gt_exp = 0;            /* value that all >1 exps must equal */

    /* factor 2 separately */
    if ((t & 1ULL) == 0) {
        size_t exp = 0;
        while ((t & 1ULL) == 0) { t >>= 1; exp++; }
        distinct_primes++;
        if (exp == 1) {
            count_lt_one_exp++;
        } else {
            count_gt_one_exp++;
            if (count_gt_one_exp == 1) expected_gt_exp = exp;
            else if (exp != expected_gt_exp) return false;
            if (exp > max_exp) max_exp = exp;
        }
    }

    /* odd primes */
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            size_t exp = 0;
            while (t % p == 0) { t /= p; exp++; }
            distinct_primes++;
            if (exp == 1) {
                count_lt_one_exp++;
            } else {
                count_gt_one_exp++;
                if (count_gt_one_exp == 1) expected_gt_exp = exp;
                else if (exp != expected_gt_exp) return false;
                if (exp > max_exp) max_exp = exp;
            }
        }
    }

    if (t > 1) {                         /* remaining prime factor */
        distinct_primes++;
        if (1 > max_exp) max_exp = 1;     /* exponent 1 */
        count_lt_one_exp++;
    }

    /* Must have at least two distinct primes … */
    if (distinct_primes < 2) return false;
    /* … at least one exponent >1 … */
    if (max_exp < 2) return false;
    /* … and at least one exponent =1 (for the "second‑largest" to be 1). */
    if (count_lt_one_exp == 0) return false;

    /* All exponents >1 must be equal; this was checked during factoring. */
    return true;
}

/* ------------------------------------------------------------------ */
/*  Generate terms up to and including index n.                      */
/* ------------------------------------------------------------------ */
static void generate_up_to(idx_t n)
{
    while (CACHE_CNT < n) {
        while (!is_valid(NEXT_VAL)) ++NEXT_VAL;
        ensure_cache_cap(CACHE_CNT + 1);
        CACHE[CACHE_CNT++] = (seq_t)NEXT_VAL;
        ++NEXT_VAL;
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */
    generate_up_to(n);
    return CACHE[n - 1];
}
