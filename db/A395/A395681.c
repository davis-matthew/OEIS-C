/*
 * A395681  a(n) is the number of positive integers m <= n such that the set
 *          of primes dividing m or dividing an exponent in the prime
 *          factorization of m is disjoint from the corresponding set for n.
 * https://oeis.org/A395681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of positive integers m ≤ n such that the set * of
 *            primes dividing m or dividing any exponent in its prime *
 *            factorization is disjoint from the corresponding set for n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint64_t idx_t;    /* index type as supplied by the build */

/* Simple dynamic set of small primes (p ≤ 65535). */
struct SSet {
    uint16_t val[32];
    uint8_t len;
};

/* check if p is already in set */
static bool set_contains(const struct SSet *s, uint16_t p)
{
    for (uint8_t i = 0; i < s->len; ++i)
        if (s->val[i] == p) return true;
    return false;
}

/* add p to set if not present */
static void set_add(struct SSet *s, uint16_t p)
{
    if (!set_contains(s, p)) {
        if (s->len < 32) s->val[s->len++] = p;
    }
}

/* compute set S(i) : primes dividing i and primes dividing any exponent in its factorization */
static void compute_Sset(uint32_t i, uint32_t *spf, struct SSet *res)
{
    res->len = 0;
    if (i <= 1) return; /* empty set for 1 */

    uint32_t n = i;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        set_add(res, (uint16_t)p);          /* add prime factor p */

        /* factor the exponent e itself */
        uint32_t e_tmp = e;
        while (e_tmp > 1) {
            uint32_t ep = spf[e_tmp];
            uint32_t cnt = 0;
            while (e_tmp % ep == 0) {
                e_tmp /= ep;
                ++cnt;
            }
            set_add(res, (uint16_t)ep);   /* add prime factor of the exponent */
        }
    }
}

/* check if two sets are disjoint */
static bool sets_disjoint(const struct SSet *a, const struct SSet *b)
{
    for (uint8_t i = 0; i < a->len; ++i) {
        uint16_t p = a->val[i];
        for (uint8_t j = 0; j < b->len; ++j)
            if (b->val[j] == p) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Allocate arrays up to n (safe because n ≤ 10 000 in tests) */
    size_t N = (size_t)n;
    uint32_t *spf = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    struct SSet *sets = (struct SSet *)malloc((N + 1) * sizeof(struct SSet));
    if (!spf || !sets) {
        /* memory allocation failed; return 0 as safety */
        free(spf);
        free(sets);
        return (seq_t)0;
    }

    /* Sieve for smallest prime factors */
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= N; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= N; ++i)
        if (spf[i] == (uint32_t)i)
            for (size_t j = i * i; j <= N; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;

    /* Compute sets for all i */
    for (size_t i = 1; i <= N; ++i)
        compute_Sset((uint32_t)i, spf, &sets[i]);

    /* Compute a(n) */
    seq_t result = (seq_t)0;
    for (size_t m = 1; m <= N; ++m)
        if (sets_disjoint(&sets[m], &sets[N])) ++result;

    /* Free memory */
    free(spf);
    free(sets);

    return result;
}
