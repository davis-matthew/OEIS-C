/*
 * A382655  Lexicographically earliest sequence of distinct positive integers
 *          such that if m = a(n-1), a(n) is the smallest novel number k such
 *          that: g = gcd(m, k) > 1, m/g > 1, k/g >1 and gcd(m/g, k) =
 *          gcd(k/g, m) = 1.
 * https://oeis.org/A382655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Lexicographically earliest sequence of distinct positive integers
   defined in OEIS A382655.  The recurrence is  
   a(1) = 6, and for n > 1, a(n) is the smallest unused integer k such that
   with m = a(n‑1) the following hold:  
     let g = gcd(m,k)  > 1,
     m/g > 1, k/g > 1,
     gcd(m/g,k) = 1  and  gcd(k/g,m) = 1.  
   This implementation memoises the sequence and uses an efficient search
   for the next term: for each proper divisor g of m that contains the full
   exponent of its constituent primes, it searches for the smallest K > 1
   coprime to m such that g*K is unused.  The smallest such candidate
   across all g is chosen as a(n).  If m has fewer than two distinct prime
   factors (which never happens for terms of this sequence) a brute‑force
   scan is performed as a fallback.  All computations are done in the
   generic integer type seq_t, using the utility functions provided by
   the build environment. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Maximum number of terms that may be generated (adjust if needed). */
#define MAXN 50000

/* Arrays to store the sequence and the set of used values.  The used
   array is kept sorted to allow binary search. */
static seq_t seq[MAXN];
static seq_t used[MAXN];
static size_t seq_len  = 0;   /* number of terms already computed
                                  a(1)…a(seq_len) */
static size_t used_len = 0;   /* size of used[] */

/* Binary search: true iff v is present in used[0 … used_len-1]. */
static bool used_contains(seq_t v)
{
    size_t l = 0, r = used_len;
    while (l < r) {
        size_t m = l + (r - l) / 2;
        if (used[m] == v) return true;
        if (used[m] < v) l = m + 1;
        else r = m;
    }
    return false;
}

/* Insert v into used[] keeping it sorted.  Assumes used_len < MAXN. */
static void used_insert(seq_t v)
{
    size_t l = 0, r = used_len;
    while (l < r) {
        size_t m = l + (r - l) / 2;
        if (used[m] < v) l = m + 1;
        else r = m;
    }
    for (size_t i = used_len; i > l; --i) used[i] = used[i - 1];
    used[l] = v;
    ++used_len;
}

/* Compute the next term after m, using the efficient divisor strategy
   with a fallback brute‐force scan. */
static seq_t next_term(seq_t m)
{
    /* Try the efficient approach first. */
    uint64_t m64 = (uint64_t)m;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64(m64, primes, exps, &pf_cnt);

    if (pf_cnt >= 2) {
        /* full prime powers for each distinct prime in m. */
        uint64_t fullp[64];
        for (size_t i = 0; i < pf_cnt; ++i) {
            uint64_t val = 1;
            for (size_t e = 0; e < exps[i]; ++e) val *= primes[i];
            fullp[i] = val;
        }

        seq_t best = 0;
        bool best_set = false;
        int total_masks = 1 << pf_cnt;          /* 2^pf_cnt */
        for (int mask = 1; mask < total_masks - 1; ++mask) {
            uint64_t g = 1;
            for (size_t i = 0; i < pf_cnt; ++i)
                if (mask & (1 << i)) g *= fullp[i];

            /* Find the smallest K > 1 coprime to m such that g*K is unused. */
            uint64_t K = 2;
            while (1) {
                /* Check coprimality with m by ensuring K is not divisible
                   by any prime factor of m. */
                bool coprime = true;
                for (size_t pi = 0; pi < pf_cnt; ++pi) {
                    if (K % primes[pi] == 0) {
                        coprime = false;
                        break;
                    }
                }
                if (!coprime) { ++K; continue; }

                seq_t candidate = (seq_t)g * (seq_t)K;
                if (!used_contains(candidate)) {
                    if (!best_set || candidate < best) {
                        best = candidate;
                        best_set = true;
                    }
                    break;          /* found candidate for this divisor g */
                }
                ++K;
            }
        }
        if (best_set) return best;
    }

    /* Fallback brute‑force search (used only for rare edge cases). */
    seq_t candidate = 2;
    while (1) {
        if (used_contains(candidate)) { ++candidate; continue; }
        seq_t g = gcd(m, candidate);
        if (g <= 1) { ++candidate; continue; }
        seq_t m_div = m / g;
        seq_t k_div = candidate / g;
        if (m_div <= 1 || k_div <= 1) { ++candidate; continue; }
        if (gcd(m_div, candidate) != 1) { ++candidate; continue; }
        if (gcd(k_div, m) != 1) { ++candidate; continue; }
        return candidate;
    }
}

/* Public function: return a(n).  memos the sequence so that calls can arrive
   in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Initialise the sequence on the first call. */
    if (seq_len == 0) {
        seq[0] = 6;
        seq_len = 1;
        used_insert(seq[0]);        /* mark 6 as used */
    }

    /* Generate terms up to the requested index. */
    for (size_t cur = seq_len + 1; cur <= (size_t)n; ++cur) {
        seq_t m = seq[seq_len - 1];      /* previous term */
        seq_t nxt = next_term(m);
        if (seq_len < MAXN) {
            seq[seq_len] = nxt;
            ++seq_len;
            used_insert(nxt);
        } else {
            /* Should not happen for the tested ranges. */
            seq[seq_len % MAXN] = nxt;
            used_insert(nxt);
        }
    }

    return seq[n - 1];
}
