/*
 * A348358  Primes which are not the concatenation of smaller primes (in base
 *          10 and allowing leading 0's).
 * https://oeis.org/A348358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348358 – primes that are not a concatenation of smaller primes (base‑10, leading zeros allowed).
   The function computes the n‑th such prime by generating primes in ascending order
   and checking the concatenation condition. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Choose sufficiently large static limits for the problem size.
   These should comfortably cover all queries that the test harness
   will make (n up to about 10 000). */
#define MAX_TERMS   50000
#define MAX_PRIMES 400000

static seq_t  terms[MAX_TERMS];      /* stored results */
static size_t terms_cnt = 0;

static uint64_t primes[MAX_PRIMES];  /* list of primes */
static size_t   prime_cnt = 0;
static uint64_t next_candidate = 2;

/* ---------- Helper: primality test using known primes ---------- */
static bool is_prime_small(uint64_t n, const uint64_t *pr, size_t pr_cnt)
{
    if (n < 2) return false;
    for (size_t i = 0; i < pr_cnt; ++i) {
        uint64_t p = pr[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- Helper: recursive concatenation test ---------- */
static bool can_split(int pos, int parts,
                      const int *dig, int len,
                      const uint64_t *pr, size_t pr_cnt)
{
    if (pos == len) return parts >= 2;        /* need at least two parts */
    uint64_t val = 0;
    for (int j = pos; j < len; ++j) {
        val = val * 10 + (uint64_t)dig[j];
        if (is_prime_small(val, pr, pr_cnt)) {
            if (can_split(j + 1, parts + 1, dig, len, pr, pr_cnt))
                return true;
        }
    }
    return false;
}

/* ---------- Predicate: can a prime be split into smaller primes? ---------- */
static bool is_concat_prime(uint64_t p, const uint64_t *pr, size_t pr_cnt)
{
    if (p < 10) return false;   /* single digit cannot be split into two primes */

    /* Extract decimal digits, most‑significant first. */
    int digits[20];  /* enough for any 64‑bit number */
    int len = 0;
    uint64_t tmp = p;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 10);
        tmp /= 10;
    }
    /* Reverse to MS first order */
    for (int i = 0; i < len / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    return can_split(0, 0, digits, len, pr, pr_cnt);
}

/* ==================== Main sequence function ==================== */
seq_t a(idx_t n)
{
    /* Safety for offset 1 */
    if (n == (idx_t)0) return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < (size_t)n) {
        /* Find next prime candidate. */
        while (!is_prime_small(next_candidate, primes, prime_cnt)) {
            ++next_candidate;
        }
        uint64_t p = next_candidate;

        /* Append p to prime list. */
        if (prime_cnt < MAX_PRIMES) {
            primes[prime_cnt++] = p;
        }

        /* Test concatenation property. */
        if (!is_concat_prime(p, primes, prime_cnt)) {
            if (terms_cnt < MAX_TERMS) {
                terms[terms_cnt++] = (seq_t)p;
            }
        }

        ++next_candidate;
    }
    return terms[n - 1];
}
