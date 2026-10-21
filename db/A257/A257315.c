/*
 * A257315  a(1) = 34; a(2) = 35; for n > 2, a(n) is the smallest number of
 *          the form prime + 32 not already used which shares a factor with
 *          a(n-1).
 * https://oeis.org/A257315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 34, a(2) = 35; for n > 2, a(n) is the smallest number of
 *            the form prime + 32 not already used which shares a factor with
 *            a(n‑1)
 * Range:     u8 n <= 16, u16 n <= 740, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"

/* ----- global tables ----- */

static idx_t *primes = NULL;      /* list of primes (ascending)          */
static size_t prime_cnt = 0;      /* number of primes stored              */
static size_t prime_cap = 0;      /* allocated capacity for primes        */
static idx_t  last_checked = 1;   /* last integer considered for primality */

static bool *used_flags = NULL;   /* true if prime+32 is already used     */
static size_t used_cap = 0;       /* allocation size of used_flags        */

static seq_t *seq_cache = NULL;   /* a(1), a(2), …, a(seq_len)           */
static size_t seq_cap = 0;        /* allocated space for seq_cache        */
static idx_t  seq_len = 0;        /* number of terms already computed     */

/* ----- helper: grow prime list ----- */
static void ensure_prime_count(size_t need)
{
    while (prime_cnt < need) {
        /* grow prime storage if necessary */
        if (prime_cnt >= prime_cap) {
            size_t new_cap = prime_cap ? (prime_cap << 1) : 16;
            idx_t *np = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
            if (!np) abort();
            primes = np;
            prime_cap = new_cap;
        }
        /* grow used_flags storage in sync */
        if (prime_cnt >= used_cap) {
            size_t new_cap = used_cap ? (used_cap << 1) : 16;
            bool *nf = (bool *)realloc(used_flags, new_cap * sizeof(bool));
            if (!nf) abort();
            used_flags = nf;
            used_cap = new_cap;
        }

        /* find next prime */
        idx_t cand;
        if (prime_cnt == 0) {
            cand = 2;
            last_checked = 2;
        } else {
            cand = last_checked + 1;
            for (;;) {
                bool is_prime = true;
                idx_t lim = (idx_t)sqrt((double)cand);
                for (size_t i = 0; i < prime_cnt && primes[i] <= lim; ++i) {
                    if (cand % primes[i] == 0) {
                        is_prime = false;
                        break;
                    }
                }
                if (is_prime) break;
                ++cand;
            }
        }
        primes[prime_cnt] = cand;
        used_flags[prime_cnt] = false;
        ++prime_cnt;
        last_checked = cand;
    }
}

/* ----- helper: grow sequence array ----- */
static void ensure_seq_cap(idx_t n)
{
    if ((size_t)n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *np = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!np) abort();
    seq_cache = np;
    seq_cap = new_cap;
}

/* ----- main sequence routine ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined (offset 1) */

    /* make sure we have at least one prime (2) */
    ensure_prime_count(1);

    /* allocate enough space for n terms */
    ensure_seq_cap(n);

    /* generate terms up to n */
    while (seq_len < n) {
        if (seq_len == 0) {                     /* a(1) = 34  */
            ensure_prime_count(1);
            seq_cache[0] = (seq_t)primes[0] + (seq_t)32;
            used_flags[0] = true;
            ++seq_len;
            continue;
        }

        if (seq_len == 1) {                     /* a(2) = 35  */
            ensure_prime_count(2);
            seq_cache[1] = (seq_t)primes[1] + (seq_t)32;
            used_flags[1] = true;
            ++seq_len;
            continue;
        }

        seq_t prev = seq_cache[seq_len - 1];

        /* search for the next unused prime+32 that shares a factor with prev */
        size_t p_idx = 0;
        for (;;) {
            ensure_prime_count(p_idx + 1);
            if (used_flags[p_idx]) {
                ++p_idx;
                continue;
            }
            seq_t cand = (seq_t)primes[p_idx] + (seq_t)32;
            if (gcd(prev, cand) != (seq_t)1) {
                seq_cache[seq_len] = cand;
                used_flags[p_idx] = true;
                break;
            }
            ++p_idx;
        }
        ++seq_len;
    }

    return seq_cache[n - 1];
}
