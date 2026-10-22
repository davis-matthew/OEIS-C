/*
 * A358565  a(n) = A358548(n) / 6.
 * https://oeis.org/A358565
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_{n+1} - p_n) / 6, where p_k is the k‑th prime ≡ 2 (mod
 *            3). Offset 2: a(2) uses the difference between the 2nd and 3rd
 *            such primes
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
/* The function build_primes_upto is provided by the utilities. */
#include "../utils/build_primes_upto.h"

static uint64_t *modprimes = NULL;      /* primes ≡ 2 (mod 3) */
static size_t modprime_count = 0;      /* number of stored primes */
static idx_t cur_limit = 0;            /* last sieve limit used */

/* Ensure we have at least need_index + 1 primes of the form 3k‑1. */
static void
ensure_primes_needed(idx_t need_index)
{
    /* need_index is n from a(n); we need primes up to index need_index+1 */
    idx_t required = need_index + 1;
    if (modprime_count > (size_t)required)
        return;                       /* already have enough */

    while (modprime_count <= (size_t)required) {
        idx_t new_limit = cur_limit == 0 ? 100 : cur_limit * 2;

        uint64_t *all = NULL;
        size_t allcnt = 0;
        build_primes_upto(new_limit, &all, &allcnt);

        /* Count primes ≡ 2 (mod 3) in the new list. */
        size_t modcnt = 0;
        for (size_t i = 0; i < allcnt; ++i)
            if (all[i] % 3 == 2)
                ++modcnt;

        /* Allocate new array for the filtered primes. */
        uint64_t *new_list = (uint64_t *)malloc(modcnt * sizeof(uint64_t));
        size_t idx = 0;
        for (size_t i = 0; i < allcnt; ++i)
            if (all[i] % 3 == 2)
                new_list[idx++] = all[i];

        free(all);
        free(modprimes);   /* discard old list */
        modprimes = new_list;
        modprime_count = modcnt;
        cur_limit = new_limit;
        /* Loop until we have enough primes. */
    }
}

seq_t
a(idx_t n)
{
    if (n < 2)          /* undefined below offset; return 0 */
        return (seq_t)0;

    ensure_primes_needed(n);

    /* The primes are stored 0‑based: modprimes[0] = 2, modprimes[1] = 5, ... */
    uint64_t diff = modprimes[(size_t)(n)] - modprimes[(size_t)(n - 1)];
    return (seq_t)(diff / 6);
}
