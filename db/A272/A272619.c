/*
 * A272619  Irregular array read by rows: n-th row contains (in ascending
 *          order) the numbers 1 <= k < n such that at least one prime divisor
 *          p of k also divides n and at least one prime divisor q of k is
 *          coprime to n.
 * https://oeis.org/A272619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9978, u16 n <= 9978, u32 n <= 9978, u64 n <= 9978,
 *            u128 n <= 9978, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A272619 – irregular array read by rows
 *
 *  A(n) is the n-th element of the flattening of an irregular array:
 *  row r (r ≥ 1) contains all 1 ≤ k < r such that
 *     (∃ prime p|k with p|r)  and  (∃ prime q|k with gcd(q,r)=1).
 *  If a row contains no such k the single term 0 is inserted.
 *
 *  The function a(idx_t n) builds rows incrementally and caches the
 *  resulting sequence.  Prime factorizations use a smallest‑prime‑factor
 *  sieve that is extended as needed.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- smallest‑prime‑factor table ----------------------------------- */
static uint32_t *spf = NULL;      /* spf[i] = smallest prime factor of i */
static size_t   spf_len = 0;      /* number of entries in spf (i=0…spf_len-1) */

/* rebuild spf table for all numbers 0 … limit */
static void build_spf(size_t limit)
{
    spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!spf) { spf_len = 0; return; }
    spf_len = limit + 1;
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;
    if (limit >= 1) spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= limit / i) {     /* avoid overflow */
                for (size_t j = i * i; j <= limit; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* return distinct prime factors of n in out[32]; *cnt holds the count */
static void get_prime_factors(idx_t n, uint32_t out[32], size_t *cnt)
{
    size_t c = 0;
    uint64_t tmp = (uint64_t)n;
    uint32_t prev = 0;
    while (tmp > 1) {
        uint32_t p = spf[(size_t)tmp];
        if (p != prev) {
            out[c++] = p;
            prev = p;
        }
        tmp /= p;
    }
    *cnt = c;
}

/* --- sequence cache ----------------------------------------------- */
static seq_t  *seq_cache = NULL;   /* cached sequence values          */
static size_t  seq_len   = 0;       /* number of stored terms          */
static size_t  seq_cap   = 0;       /* capacity of the cache           */
static idx_t   last_row  = 0;       /* last processed row index        */

/* append a value to the dynamic cache */
static void append_val(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *newp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!newp) return;          /* allocation failed – abort silently */
        seq_cache = newp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* generate rows until the cache contains at least n terms */
    while (seq_len < n) {
        ++last_row;                     /* next row index */

        if (last_row < 2) {             /* rows 1 and 0 have no k */
            append_val((seq_t)0);
            continue;
        }

        /* ensure spf table covers the current row number */
        if ((size_t)last_row + 1 > spf_len)
            build_spf((size_t)last_row);

        /* prime factors of the current row number */
        uint32_t primes[32];
        size_t pf_cnt;
        get_prime_factors(last_row, primes, &pf_cnt);

        /* scan all k < last_row and collect suitable ones */
        bool row_has_k = false;
        for (uint64_t k = 2; k < last_row; ++k) {
            bool has_div   = false;
            bool has_copr  = false;
            uint64_t tmp   = k;
            uint32_t prev_p = 0;

            while (tmp > 1) {
                uint32_t p = spf[(size_t)tmp];
                if (p != prev_p) {          /* distinct factor */
                    bool divides = false;
                    for (size_t i = 0; i < pf_cnt; ++i)
                        if (primes[i] == p) { divides = true; break; }
                    if (divides) has_div = true;
                    else        has_copr = true;
                    if (has_div && has_copr) break;
                    prev_p = p;
                }
                tmp /= p;
            }
            if (has_div && has_copr) {
                append_val((seq_t)k);
                row_has_k = true;
            }
        }
        if (!row_has_k)                /* empty row -> single 0 */
            append_val((seq_t)0);
    }
    return seq_cache[n - 1];
}
