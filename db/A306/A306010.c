/*
 * A306010  Let S(m) = d(k)/d(1) + ... + d(1)/d(k), where d(1)..d(k) are the
 *          unitary divisors of m; then a(n) is the number m when the sums
 *          S(m) are arranged in increasing order.
 * https://oeis.org/A306010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the permutation of positive integers defined by ordering
 * the values S(m) = sum_{d|m, gcd(d,m/d)=1} d/(m/d) for all m >= 1.
 * The implementation builds the values for all m up to a fixed
 * limit (120000) and sorts them by the rational S(m).  The n-th
 * term of the sequence equals the m at position n in this sorted list.
 */

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    __uint128_t num;   /* numerator of S(m) */
    uint32_t    den;   /* denominator = m */
    uint32_t    m;     /* the integer m itself */
} item_t;

/* Compare two items by their S(m) values (num/den).  On tie, sort by m. */
static int cmp_item(const void *p1, const void *p2)
{
    const item_t *a = (const item_t *)p1;
    const item_t *b = (const item_t *)p2;
    __uint128_t left  = a->num * b->den;
    __uint128_t right = b->num * a->den;
    if (left < right) return -1;
    if (left > right) return 1;
    /* Ties: smaller m first. */
    return (int)(a->m - b->m);
}

seq_t a(idx_t n)
{
    /* Fixed upper bound sufficient for n up to about 10000. */
    const size_t LIMIT = 120000;

    /* Lazy static initialization of the sorted list. */
    static item_t *items = NULL;
    static uint32_t *spf   = NULL;  /* smallest prime factor sieve */
    static int initialized = 0;

    if (!initialized)
    {
        /* Allocate arrays. */
        items = (item_t *)malloc(LIMIT * sizeof(item_t));
        if (!items) return 0; /* allocation failure, return 0 */
        spf   = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
        if (!spf) { free(items); items = NULL; return 0; }

        /* Compute smallest prime factor for each number up to LIMIT. */
        for (uint32_t i = 2; i <= LIMIT; ++i)
        {
            if (!spf[i])
            {
                spf[i] = i;
                if ((uint64_t)i * i <= LIMIT)
                {
                    for (uint32_t j = i * i; j <= LIMIT; j += i)
                        if (!spf[j]) spf[j] = i;
                }
            }
        }

        /* Build the items array for all m. */
        for (uint32_t m = 1; m <= LIMIT; ++m)
        {
            uint32_t tmp = m;
            __uint128_t numerator = 1;

            while (tmp > 1)
            {
                uint32_t p = spf[tmp];
                uint32_t e = 0;
                while (tmp % p == 0)
                {
                    tmp /= p;
                    ++e;
                }
                /* Compute p^(2*e) + 1 */
                __uint128_t pow = 1;
                for (uint32_t i = 0; i < 2 * e; ++i) pow *= p;
                numerator *= (pow + 1);
            }

            items[m - 1].num = numerator;
            items[m - 1].den = m;
            items[m - 1].m   = m;
        }

        /* Sort by S(m). */
        qsort(items, LIMIT, sizeof(item_t), cmp_item);

        initialized = 1;
    }

    /* Bounds check. */
    if (n == 0 || (size_t)n > LIMIT) return 0;
    return (seq_t)items[n - 1].m;
}
