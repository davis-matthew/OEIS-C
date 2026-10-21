/*
 * A243463  Numbers n such that prime(n) and phi(n) have no common digits.
 * https://oeis.org/A243463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   digit_mask_uint64, get_prime, grow_seq, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the n‑th integer k such that prime(k) and phi(k) share no digits. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/get_prime.h"
#include "../utils/phi.h"
#include "../utils/digit_mask_uint64.h"

static seq_t *cache = NULL;        /* cached terms of the sequence   */
static size_t cache_len = 0;       /* number of cached terms         */
static size_t cache_cap = 0;       /* current capacity of 'cache'    */
static uint64_t tested_up_to = 0;  /* next candidate n to test (1‑based) */

/* ensure that 'cache' contains at least 'target' terms */
static void ensure_to(idx_t target)
{
    if ((size_t)target <= cache_len)
        return;                     /* already have enough terms */

    if (!cache)                     /* allocate initial buffer   */
        grow_seq(&cache, &cache_cap, (size_t)target);

    while (cache_len < (size_t)target)
    {
        uint64_t n = ++tested_up_to;       /* candidate integer             */
        seq_t  phi_n  = phi(n);
        uint64_t phi_val = (uint64_t)phi_n;  /* safe: phi(n) ≤ n ≤ 2^64‑1     */
        uint32_t prime_val = get_prime(n);   /* nth prime, 1‑indexed            */

        uint16_t mask_p = digit_mask_uint64((uint64_t)prime_val);
        uint16_t mask_t = digit_mask_uint64(phi_val);

        if ((mask_p & mask_t) == 0)  /* no common decimal digit */
        {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)n;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        abort();                     /* sequence offset is 1            */
    ensure_to(n);
    return cache[n - 1];
}
