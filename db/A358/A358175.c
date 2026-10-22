/*
 * A358175  a(1) = 1, a(2) = 2; a(3) = 3; for n > 3, a(n) is the smallest
 *          positive number not occurring earlier that shares a factor with
 *          Sum_{k=1..n-1} A001065(a(k)), where A001065(m) is the sum of the
 *          proper divisors of m.
 * https://oeis.org/A358175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 38, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358175
   a(1)=1, a(2)=2, a(3)=3.
   For n>3, a(n) is the smallest unused positive integer that shares a
   factor with S(n-1) = Σ_{k=1}^{n-1} A001065(a(k)),
   where A001065(m) = Σ_{d|m, d<m} d (sum of proper divisors). */

/* -------------------------------------------------------------------- */
/*  Standard headers (C11 and C++17 compatible)                         */
/* -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* -------------------------------------------------------------------- */
/*  Static state (memoisation)                                          */
/* -------------------------------------------------------------------- */
static bool          initialized = false;
static seq_t        *sequence    = NULL;   /* 1‑based sequence */
static size_t        seq_cap     = 0;
static size_t        seq_len     = 0;       /* terms produced so far */

static bool        *used        = NULL;   /* used[i] == true if i used */
static size_t        used_cap    = 0;

static uint32_t    *prime_list  = NULL;   /* primes up to prime_limit */
static size_t        prime_cnt   = 0;
static size_t        prime_cap   = 0;

static seq_t       *prime_small = NULL;   /* next unused multiple per prime */
static size_t        prime_small_cap = 0;

static seq_t        current_sum = 0;      /* Σ A001065(a(k)) */

/* -------------------------------------------------------------------- */
/*  Helpers for dynamic arrays                                         */
/* -------------------------------------------------------------------- */
static void extend_used(size_t cap)          /* cap is at least index+1   */
{
    if (cap <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 128;
    while (new_cap < cap) new_cap <<= 1;
    bool *newp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!newp) exit(EXIT_FAILURE);
    memset(newp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used    = newp;
    used_cap = new_cap;
}

static void extend_sequence(size_t cap)
{
    if (cap <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
    if (!newp) exit(EXIT_FAILURE);
    sequence = newp;
    seq_cap  = new_cap;
}

static void extend_prime_list(size_t cap)
{
    if (cap <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 128;
    while (new_cap < cap) new_cap <<= 1;
    uint32_t *newp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!newp) exit(EXIT_FAILURE);
    prime_list = newp;
    prime_cap  = new_cap;
}

/* -------------------------------------------------------------------- */
/*  Prime utilities                                                     */
/* -------------------------------------------------------------------- */
static size_t find_prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t v = prime_list[mid];
        if (v == p) return mid;
        if (v < p) lo = mid + 1;
        else hi = mid;
    }
    return SIZE_MAX;
}

static void build_primes(uint32_t limit)
{
    size_t expected = limit / 10;
    extend_prime_list(expected);

    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);

    prime_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            extend_prime_list(prime_cnt + 1);
            prime_list[prime_cnt++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);

    prime_small_cap = prime_cnt;
    prime_small = (seq_t *)malloc(prime_small_cap * sizeof(seq_t));
    if (!prime_small) exit(EXIT_FAILURE);
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_small[i] = (seq_t)prime_list[i];
}

/* -------------------------------------------------------------------- */
/*  Mark a value as used and advance prime_small candidates            */
/* -------------------------------------------------------------------- */
static void mark_used(seq_t x)
{
    idx_t ix = (idx_t)x;
    extend_used((size_t)ix + 1);

    used[(size_t)ix] = true;
    if (x < 2) return;

    uint64_t tmp = (uint64_t)x;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            size_t pi = i;
            seq_t cand = prime_small[pi];
            while (1) {
                if ((size_t)cand >= used_cap)
                    extend_used((size_t)cand + 1);
                if (!used[(size_t)cand]) break;
                cand += (seq_t)p;
            }
            prime_small[pi] = cand;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        uint32_t p = (uint32_t)tmp;
        size_t pi = find_prime_index(p);
        if (pi != SIZE_MAX) {
            seq_t cand = prime_small[pi];
            while (1) {
                if ((size_t)cand >= used_cap)
                    extend_used((size_t)cand + 1);
                if (!used[(size_t)cand]) break;
                cand += (seq_t)p;
            }
            prime_small[pi] = cand;
        }
    }
}

/* -------------------------------------------------------------------- */
/*  Computing A001065 (sum of proper divisors)                         */
/* -------------------------------------------------------------------- */
static seq_t proper_divisor_sum(uint64_t m)
{
    if (m <= 1) return 0;
    uint64_t tmp = m;
    seq_t prod = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            int  e   = 0;
            seq_t pow_p = 1;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
                pow_p *= (seq_t)p;
            }
            seq_t numerator   = pow_p * (seq_t)p - 1;
            seq_t denominator = (seq_t)p - 1;
            seq_t term = numerator / denominator;
            prod *= term;
        }
    }
    if (tmp > 1) {
        seq_t p = (seq_t)tmp;
        seq_t numerator   = p * p - 1;
        seq_t denominator = p - 1;
        seq_t term = numerator / denominator;
        prod *= term;
    }
    return prod - (seq_t)m;
}

/* -------------------------------------------------------------------- */
/*  Find smallest unused candidate sharing a factor with sum          */
/* -------------------------------------------------------------------- */
static seq_t smallest_candidate(seq_t sum)
{
    uint64_t n = (uint64_t)sum;
    uint64_t tmp = n;
    seq_t best = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            size_t pi = i;
            seq_t cand = prime_small[pi];
            while (1) {
                if ((size_t)cand >= used_cap)
                    extend_used((size_t)cand + 1);
                if (!used[(size_t)cand]) break;
                cand += (seq_t)p;
            }
            if (!best || cand < best) best = cand;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {     /* remaining prime factor */
        uint32_t p = (uint32_t)tmp;
        seq_t cand;
        size_t pi = find_prime_index(p);
        if (pi != SIZE_MAX) {
            cand = prime_small[pi];
            while (1) {
                if ((size_t)cand >= used_cap)
                    extend_used((size_t)cand + 1);
                if (!used[(size_t)cand]) break;
                cand += (seq_t)p;
            }
        } else {
            cand = (seq_t)p;
            while (1) {
                if ((size_t)cand >= used_cap)
                    extend_used((size_t)cand + 1);
                if (!used[(size_t)cand]) break;
                cand += (seq_t)p;
            }
        }
        if (!best || cand < best) best = cand;
    }
    return best;
}

/* -------------------------------------------------------------------- */
/*  Initialise once                                                 */
/* -------------------------------------------------------------------- */
static void initialise(void)
{
    if (initialized) return;
    initialized = true;

    build_primes(400000);   /* enough for n ≤ 10000 */

    used_cap = 128;
    used = (bool *)calloc(used_cap, sizeof(bool));
    if (!used) exit(EXIT_FAILURE);

    seq_cap = 128;
    sequence = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    if (!sequence) exit(EXIT_FAILURE);

    sequence[1] = 1; mark_used(1);
    sequence[2] = 2; mark_used(2);
    sequence[3] = 3; mark_used(3);
    seq_len = 3;

    current_sum = 0;
    for (size_t i = 1; i <= 3; ++i)
        current_sum += proper_divisor_sum((uint64_t)sequence[i]);
}

/* -------------------------------------------------------------------- */
/*  Public function                                                     */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    initialise();

    if ((size_t)n <= seq_len) return sequence[(size_t)n];

    while (seq_len < (size_t)n) {
        ++seq_len;
        extend_sequence(seq_len + 1);
        if (seq_len <= 3) {               /* safety, though already seeded */
            sequence[seq_len] = (seq_t)seq_len;
            mark_used(sequence[seq_len]);
            current_sum += proper_divisor_sum((uint64_t)seq_len);
            continue;
        }
        seq_t cand = smallest_candidate(current_sum);
        sequence[seq_len] = cand;
        mark_used(cand);
        current_sum += proper_divisor_sum((uint64_t)cand);
    }
    return sequence[(size_t)n];
}
