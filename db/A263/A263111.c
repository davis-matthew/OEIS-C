/*
 * A263111  Ordinal transform of the arithmetic derivative A003415.
 * https://oeis.org/A263111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ j ∈ [0,n] : d(j) = d(n) }|
 * Range:     u8 n <= 421, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* d is the arithmetic derivative A003415 */
#include <stdlib.h>

static uint64_t *spf = NULL;     /* smallest prime factor table */
static idx_t spf_limit = 0;       /* highest index for which spf is filled */

/* fill spf[0..limit] using a simple sieve */
static void sieve_spf(idx_t limit, uint64_t *spf)
{
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;
    spf[0] = 0;
    if (limit >= 1)
        spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }
}

/* arithmetic derivative of a non‑negative integer x using spf table   */
/* for x ≥ 2 : d(x) = Σ e_i * x / p_i  (p_i^e_i | x)                */
static seq_t derivative_of(idx_t x, uint64_t *spf)
{
    if (x < 2)
        return (seq_t)0;
    seq_t result = 0;
    seq_t n_seq = (seq_t)x;      /* keep the original value for the formula */
    idx_t temp = x;
    while (temp > 1) {
        uint64_t p = spf[temp];
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        /* add e * n / p  */
        result += (n_seq / (seq_t)p) * (seq_t)exp;
    }
    return result;
}

/* public sequence function required by the harness */
seq_t a(idx_t n)
{
    /* make sure spf table covers [0,n] */
    if (n > spf_limit) {
        uint64_t *new_spf = (uint64_t *)realloc(spf, sizeof(uint64_t) * (n + 1));
        if (!new_spf)   /* allocation failure: best effort, return 0 */
            return (seq_t)0;
        spf = new_spf;
        spf_limit = n;
        sieve_spf(spf_limit, spf);
    }

    seq_t dn = derivative_of(n, spf);

    /* count indices j ∈ [0,n] with the same derivative */
    seq_t count = 0;
    for (idx_t j = 0; j <= n; ++j) {
        if (derivative_of(j, spf) == dn)
            count++;
    }
    return count;
}
