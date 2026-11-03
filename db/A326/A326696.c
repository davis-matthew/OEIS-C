/*
 * A326696  Numbers m with at least one divisor d > 1 such that sigma(d)
 *          divides m.
 * https://oeis.org/A326696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A326696 – numbers m with at least one divisor d>1 such that
 *            sigma(d) divides m
 * Range:     u8 n <= 43, u16 n <= 6214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9889 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Recursive search over all divisors of m.  
   cur_div   : current divisor value (idx_t)  
   sigma_cur : sigma(cur_div) as seq_t */
static bool dfs_has_divisor(seq_t m_seq, uint64_t *pr, uint64_t *ex, size_t cnt,
                            size_t pos, idx_t cur_div, seq_t sigma_cur)
{
    if (pos == cnt) {
        if (cur_div > (idx_t)1 && (m_seq % sigma_cur) == (seq_t)0)
            return true;
        return false;
    }

    uint64_t p   = pr[pos];
    int    max_e = (int)ex[pos];

    /* Case e = 0 – divisor unchanged */
    if (dfs_has_divisor(m_seq, pr, ex, cnt, pos + 1, cur_div, sigma_cur))
        return true;

    /* Iterate e = 1 .. max_e */
    idx_t  cur   = cur_div;
    seq_t  sigma_p = 1;   /* sigma(p^0) */

    for (int e = 1; e <= max_e; ++e) {
        cur   *= p;
        sigma_p = sigma_p * (seq_t)p + 1;      /* sigma(p^e) from sigma(p^(e−1)) */
        seq_t new_sigma = sigma_cur * sigma_p;
        if (dfs_has_divisor(m_seq, pr, ex, cnt, pos + 1, cur, new_sigma))
            return true;
    }
    return false;
}

/* Memoised storage of the sequence */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static idx_t  next_check = 2;

/* Ensure the terms array has room for one more element */
static void ensure_terms_capacity(void)
{
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = mem_new(seq_t, terms_cap);
    } else if (terms_len == terms_cap) {
        terms_cap <<= 1;
        terms = mem_resize(seq_t, terms, terms_cap);
    }
}

/* Return true iff m has a divisor d>1 with sigma(d) | m. */
static bool has_good_divisor(idx_t m)
{
    seq_t m_seq = (seq_t)m;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);
    return dfs_has_divisor(m_seq, primes, exps, cnt, 0, 1, 1);
}

/* Generate terms until at least n terms are stored. */
static void generate_up_to(idx_t n)
{
    while (terms_len < n) {
        if (has_good_divisor(next_check)) {
            ensure_terms_capacity();
            terms[terms_len++] = (seq_t)next_check;
        }
        ++next_check;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1; return 0 */
    generate_up_to(n);
    return terms[n - 1];
}
