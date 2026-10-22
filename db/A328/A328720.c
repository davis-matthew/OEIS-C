/*
 * A328720  The position function the fractalization of which yields A328719.
 * https://oeis.org/A328720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of n in the list {1..n} sorted lexicographically by
 *            their vector of p‑adic valuations. The lexicographic order
 *            compares the exponents of primes 2,3,5,... in increasing order
 *            of the primes. Implementation details: For each prime factor p
 *            of n with exponent e, for all a in 0..e-1 count...
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *spf = NULL;
static size_t spf_limit = 0;

/* Update spf array to cover all numbers up to new_limit. */
static void ensure_spf(size_t new_limit)
{
    if (new_limit <= spf_limit) return;
    uint32_t *new_spf = (uint32_t*)calloc(new_limit + 1, sizeof(uint32_t));
    if (!new_spf) exit(1); /* allocation failed */
    new_spf[1] = 1;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = (uint32_t)i;
            if (i > new_limit / i) continue; /* avoid overflow */
            for (size_t j = i * i; j <= new_limit; j += i) {
                if (!new_spf[j]) new_spf[j] = (uint32_t)i;
            }
        }
    }
    if (spf) free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* Small power function for seq_t.  exponents are small (≤ log₂(n)). */
static seq_t pow_seq_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Count integers x, 1 ≤ x ≤ limit, whose smallest prime factor > p
   (equivalently, all prime factors > p).  For x = 1, count it. */
static idx_t count_coprimes_gt_p(idx_t limit, idx_t p)
{
    idx_t cnt = 0;
    for (idx_t x = 1; x <= limit; ++x) {
        if (x == 1) { cnt++; continue; }
        if (spf[x] > p) cnt++;
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 */

    ensure_spf((size_t)n);         /* spf up to n */

    /* Factor n into primes with exponents (ascending primes). */
    idx_t temp = n;
    struct { idx_t p; idx_t e; } fac[64];
    size_t fac_cnt = 0;

    while (temp > 1) {
        idx_t p = spf[temp];
        if (p == 0) p = temp;          /* safety */
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        fac[fac_cnt].p = p;
        fac[fac_cnt].e = e;
        ++fac_cnt;
    }

    seq_t rank = 1;          /* 1‑based rank */
    seq_t prefix_product = 1;

    for (size_t i = 0; i < fac_cnt; ++i) {
        idx_t p = fac[i].p;
        idx_t e = fac[i].e;

        seq_t pow_a = 1;     /* p^a, starting with a=0 */
        for (idx_t a = 0; a < e; ++a) {
            seq_t prod_prefix = prefix_product * pow_a;
            idx_t remaining = (idx_t)((seq_t)n / prod_prefix);
            idx_t cnt = count_coprimes_gt_p(remaining, p);
            rank += (seq_t)cnt;
            pow_a *= (seq_t)p;   /* next power of p */
        }

        seq_t pow_e = pow_seq_small((seq_t)p, e);
        prefix_product *= pow_e;
    }

    return rank;
}
