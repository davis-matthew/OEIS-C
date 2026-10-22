/*
 * A328470  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A046523(i) = A046523(j) and A053669(i) = A053669(j) for all i,
 *          j.
 * https://oeis.org/A328470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lexicographically earliest restricted growth *
 *            sequence of the pair (prime_signature(n),
 *            smallest_prime_not_dividing(n)), * where prime_signature(n) is
 *            the multiset of prime exponents of n, * and
 *            smallest_prime_not_dividing(n) is the smallest prime that does
 *            not * divide n. The first...
 * Range:     u8 n <= 9245, u16 n <= 9245, u32 n <= 9245, u64 n <= 9245,
 *            u128 n <= 9245, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

struct Entry {
    uint32_t sig;   /* prime-exponent signature encoded */
    uint16_t spnp;  /* smallest prime not dividing n */
    seq_t    val;   /* value assigned to this class */
};

#define MAX_CLASSES 20000   /* enough for n <= 10 000 */

static struct Entry entries[MAX_CLASSES];
static size_t       e_count      = 0;   /* number of distinct classes seen */
static seq_t        next_val     = 1;   /* next value to assign to a new class */
static idx_t        premax       = 0;   /* highest index computed so far */

/* Compute an integer code for the multiset of exponents of n.
 * The code is built by sorting the exponents in non‑increasing order
 * and packing each exponent (≤16) into 5 bits. */
static uint32_t prime_sig_id(uint64_t n)
{
    if (n <= 1) return 0;   /* 1 has signature 0, which is unique */

    uint64_t   pr[16];
    uint64_t   ex[16];
    size_t     cnt;
    factor_u64(n, pr, ex, &cnt);

    uint32_t ex_sorted[16];
    for (size_t i = 0; i < cnt; ++i)
        ex_sorted[i] = (uint32_t)ex[i];

    /* sort descending (cnt <= 16 for n <= 10 000) */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (ex_sorted[i] < ex_sorted[j]) {
                uint32_t tmp = ex_sorted[i];
                ex_sorted[i] = ex_sorted[j];
                ex_sorted[j] = tmp;
            }

    uint32_t sig = 0;
    for (size_t i = 0; i < cnt; ++i)
        sig = (sig << 5) | ex_sorted[i];

    return sig;
}

/* Smallest prime that does not divide n. */
static uint16_t smallest_prime_not_dividing(uint64_t n)
{
    uint64_t p = 2;
    while ((n % p) == 0)
        p = next_prime(p);   /* next_prime returns next prime > p */
    return (uint16_t)p;
}

/* Extend precomputation up to index n (inclusive). */
static void precompute_up_to(idx_t n)
{
    if (n <= premax)
        return;

    for (idx_t i = premax + 1; i <= n; ++i)
    {
        uint64_t u   = (uint64_t)i;
        uint32_t sig = prime_sig_id(u);
        uint16_t spnp = smallest_prime_not_dividing(u);

        seq_t assigned = 0;   /* 0 means not yet found */

        for (size_t j = 0; j < e_count; ++j)
        {
            if (entries[j].sig == sig && entries[j].spnp == spnp)
            {
                assigned = entries[j].val;
                break;
            }
        }

        if (assigned == 0)                /* new class */
        {
            /* In practice e_count never exceeds MAX_CLASSES for n <= 10 000. */
            entries[e_count].sig   = sig;
            entries[e_count].spnp  = spnp;
            entries[e_count].val   = next_val;
            assigned = next_val;
            ++next_val;
            ++e_count;
        }
    }

    premax = n;
}

/* Public function requested by the judge. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, safety value */

    precompute_up_to(n);

    /* Retrieve the value assigned to this index. */
    uint64_t u   = (uint64_t)n;
    uint32_t sig = prime_sig_id(u);
    uint16_t spnp = smallest_prime_not_dividing(u);

    for (size_t j = 0; j < e_count; ++j)
    {
        if (entries[j].sig == sig && entries[j].spnp == spnp)
            return entries[j].val;
    }

    /* Should never reach here. */
    return 0;
}
