/*
 * A355537  Number of ways to choose a sequence of prime factors, one of each
 *          integer from 2 to n.
 * https://oeis.org/A355537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 34, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A355537: Number of ways to choose a sequence of prime factors,
    one of each integer from 2 to n.
    a(n) = ∏_{i=2}^n ω(i)   where ω(i) is the number of distinct prime
    factors of i.
    The implementation builds a smallest‑prime‑factor (spf) table and
    caches the prefix products.
    It works for all n ≥ 1, is O(n) for each new maximum n, and
    uses only allowed arithmetic on seq_t and idx_t. */
#include <stdlib.h>

static idx_t *spf        = NULL;   /* smallest prime factor table           */
static size_t spf_size   = 0;      /* allocated length of spf               */
static seq_t *prod       = NULL;   /* prefix products a(k) for k≤n          */
static size_t prod_size  = 0;      /* allocated length of prod              */
static idx_t  computed_up_to = 0;  /* largest n for which prod is filled */

/* Build or resize the spf table up to at least limit. */
static void build_spf(idx_t limit)
{
    if (spf_size < (size_t)(limit + 1)) {
        idx_t *new_spf = (idx_t *)realloc(spf, sizeof(idx_t) * (size_t)(limit + 1));
        if (!new_spf) { free(spf); free(prod); /* terminate on bad malloc */ }
        spf = new_spf;
        spf_size = (size_t)(limit + 1);
        for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
        for (idx_t i = 2; i * i <= limit; ++i)
            if (spf[i] == i) /* i is prime */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == j) spf[j] = i;
    }
}

/* Ensure tables are large enough and prefix products up to n are
   computed.  */
static void ensure_up_to(idx_t n)
{
    if (n <= (idx_t)computed_up_to) return;

    /* Grow the SPF table if necessary. */
    build_spf(n);

    /* Grow the products table if necessary. */
    if (prod_size < (size_t)(n + 1)) {
        seq_t *new_prod = (seq_t *)realloc(prod, sizeof(seq_t) * (size_t)(n + 1));
        if (!new_prod) { free(spf); free(prod); /* terminate on bad malloc */ }
        prod = new_prod;
        prod_size = (size_t)(n + 1);
    }

    /* Base cases for the prefix product. */
    if (computed_up_to == 0) {
        prod[0] = (seq_t)1;
        if (n >= 1) prod[1] = (seq_t)1;
        computed_up_to = 1;
    }

    /* Compute products for new indices. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        idx_t x = i;
        idx_t last_prime = 0;
        seq_t omega = 0;             /* distinct prime factors of i */
        while (x > 1) {
            idx_t p = spf[x];
            if (p != last_prime) {
                ++omega;
                last_prime = p;
            }
            /* divide out all copies of p */
            do { x /= p; } while (x % p == 0);
        }
        prod[i] = prod[i - 1] * omega;
    }

    computed_up_to = n;
}

/* Public function required by the driver. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* sequence is defined for n≥1 only */
    ensure_up_to(n);
    return prod[n];
}
