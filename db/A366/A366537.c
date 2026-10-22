/*
 * A366537  The sum of unitary divisors of the cubefree numbers (A004709).
 * https://oeis.org/A366537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A034448(A004709(n)) i.e. the sum of unitary divisors of
 *            the n‑th cubefree number
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *spf = NULL;         /* smallest prime factor array */
static idx_t spf_limit = 0;

static uint64_t *cubefree = NULL;    /* list of cubefree numbers in order */
static idx_t cf_len  = 0;            /* number of cubefree numbers stored */
static idx_t cf_cap  = 0;            /* allocated capacity of cubefree array */

/* build spf array up to `limit` and fill cubefree array up to that limit */
static void build_sieve_and_cubefree(idx_t limit)
{
    /* free old memory if any */
    free(spf);
    free(cubefree);

    /* allocate spf */
    spf = (uint64_t*)calloc((size_t)limit + 1, sizeof(uint64_t));
    if (!spf) exit(EXIT_FAILURE);   /* memory error */

    /* simple sieve to fill spf */
    for (uint64_t i = 2; i <= (uint64_t)limit; ++i)
    {
        if (!spf[i])
        {
            spf[i] = i;
            if (i <= limit / i)   /* avoid overflow of i*i */
            {
                for (uint64_t j = i * i; j <= (uint64_t)limit; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }
    spf[1] = 1;

    /* allocate cubefree array capable of holding all numbers up to limit */
    cf_cap = limit;
    cubefree = (uint64_t*)malloc((size_t)cf_cap * sizeof(uint64_t));
    if (!cubefree) exit(EXIT_FAILURE);

    /* generate cubefree numbers up to limit */
    cf_len = 0;
    for (uint64_t num = 1; num <= (uint64_t)limit; ++num)
    {
        uint64_t tmp = num;
        int cube_free = 1;
        while (tmp > 1)
        {
            uint64_t p = spf[tmp];
            int e = 0;
            while (tmp % p == 0)
            {
                ++e;
                tmp /= p;
                if (e >= 3) { cube_free = 0; break; }
            }
            if (!cube_free) break;
        }
        if (cube_free)
        {
            if (cf_len == cf_cap) /* this should not happen due to capacity = limit */
            {
                cf_cap *= 2;
                cubefree = (uint64_t*)realloc(cubefree, (size_t)cf_cap * sizeof(uint64_t));
                if (!cubefree) exit(EXIT_FAILURE);
            }
            cubefree[cf_len++] = num;
        }
    }

    spf_limit = limit;
}

/* ensure that the caches contain at least n cubefree numbers */
static void ensure_cubefree(idx_t n)
{
    if (cf_len >= n) return;

    /* approximate required limit using density 1/zeta(3) ≈ 0.831 */
    idx_t limit = n * 2 + 100;          /* rough upper bound */
    while (1)
    {
        build_sieve_and_cubefree(limit);
        if (cf_len >= n) break;
        limit *= 2;                     /* increase and rebuild */
    }
}

/* compute sum of unitary divisors of a cubefree number m */
static seq_t sigma_star_of(uint64_t m)
{
    seq_t prod = 1;
    uint64_t tmp = m;
    while (tmp > 1)
    {
        uint64_t p = spf[tmp];
        unsigned e = 0;
        while (tmp % p == 0)
        {
            ++e;
            tmp /= p;
        }
        seq_t term = 1;
        seq_t pseq = (seq_t)p;
        if (e == 2)
            term += pseq * pseq;
        else
            term += pseq;
        prod *= term;
    }
    return prod;
}

/* ------------------------------------------------------------------------- */
/* External function requested by the test harness.                         */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of bounds – not used by the tests */
    ensure_cubefree(n);
    uint64_t m = cubefree[n - 1];
    return sigma_star_of(m);
}
