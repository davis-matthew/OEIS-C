/*
 * A317841  Möbius transform of A284013 (= n - A002487(n)), where A002487 is
 *          Stern's Diatomic sequence.
 * https://oeis.org/A317841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * ( n/d – Stern(n/d) ) where Stern(0)=0,
 *            Stern(1)=1, Stern(2k)=Stern(k), Stern(2k+1)=Stern(k)+Stern(k+1)
 * Range:     u8 n <= 306, u16 n <= 9684, u32 n <= 9684, u64 n <= 9684,
 *            u128 n <= 9684, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Cached Stern values: 0..stern_cap-1 */
static seq_t *stern_cache = NULL;
static idx_t stern_cap   = 0;

/* Ensure stern_cache can hold indices up to n (inclusive). */
static void ensure_stern(idx_t n)
{
    if (stern_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) return; /* allocation failed – leave cache empty */
        if (stern_cache) {
            memcpy(new_cache, stern_cache, sizeof(seq_t) * stern_cap);
            free(stern_cache);
        }
        /* initialise new entries to zero */
        memset(new_cache + stern_cap, 0, sizeof(seq_t) * (new_cap - stern_cap));
        /* set base value */
        if (new_cap > 1) new_cache[1] = (seq_t)1;
        stern_cache = new_cache;
        stern_cap   = new_cap;
    }
}

/* Recursive Stern(n) with cache. Stern(0)=0, Stern(1)=1. */
static seq_t stern_val(idx_t m)
{
    if (m == 0) return 0;
    if (m == 1) return 1;                /* base case, cached elsewhere */

    if (stern_cache[m] != 0) return stern_cache[m];

    if ((m & 1) == 0) {
        stern_cache[m] = stern_val(m >> 1);
    } else {
        stern_cache[m] = stern_val(m >> 1) + stern_val((m >> 1) + 1);
    }
    return stern_cache[m];
}

/* Möbius function μ(n) – simple trial division. */
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    int cnt = 0;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;  ++e;
                if (e > 1) return 0;    /* squared prime factor */
            }
            ++cnt;                      /* distinct prime */
        }
    }
    if (n > 1) ++cnt;                      /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* defined only for n ≥ 1 */

    ensure_stern(n);                        /* Stern cache up to n */

    long long sum = 0;                      /* accumulate as signed long long */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* divisor d ------------------------------------------------- */
        {
            int mu = mobius(d);
            if (mu != 0) {
                idx_t q = n / d;
                seq_t stern_q = stern_val(q);
                seq_t val = (seq_t)q - stern_q; /* A284013(q) */
                if (mu > 0) sum += (long long)val;
                else        sum -= (long long)val;
            }
        }

        /* paired divisor d2 = n/d ------------------------------------ */
        idx_t d2 = n / d;
        if (d2 != d) {
            int mu = mobius(d2);
            if (mu != 0) {
                idx_t q = n / d2;            /* q == d */
                seq_t stern_q = stern_val(q);
                seq_t val = (seq_t)q - stern_q; /* A284013(q) */
                if (mu > 0) sum += (long long)val;
                else        sum -= (long long)val;
            }
        }
    }
    return (seq_t)sum;
}
