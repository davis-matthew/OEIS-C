/*
 * A376543  Numbers of the form p^e * q^f with p, q distinct primes = 3 mod 4
 *          and e and f both odd.
 * https://oeis.org/A376543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   numbers of the form p^e * q^f with p, q distinct primes ≡ 3 (mod
 *            4) and e,f odd
 * Range:     u8 n <= 19, u16 n <= 3936, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9722 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Check if a 64‑bit integer has the required form. */
static bool is_blum(uint64_t m)
{
    if (m < 21)                 /* smallest term is 3·7 = 21 */
        return false;

    uint64_t n = m;
    int cnt = 0;

    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (p % 4 != 3)            /* prime must be 3 mod 4 */
                return false;
            if ((exp & 1) == 0)        /* exponent must be odd */
                return false;
            ++cnt;
            if (cnt > 2)
                return false;
        }
    }

    if (n > 1) {                /* remaining factor is prime */
        if (n % 4 != 3)
            return false;
        ++cnt;
        if (cnt > 2)
            return false;
    }

    return cnt == 2;
}

/* Ensure that an array has capacity at least min_cap. */
static void ensure_capacity(size_t min_cap, seq_t **arr, size_t *cap)
{
    if (*cap < min_cap) {
        size_t new_cap = (*cap == 0 ? 16 : (*cap << 1));
        if (new_cap < min_cap)
            new_cap = min_cap;
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_candidate = 3;   /* start checking from 3 */

    while (seq_len < n)
    {
        uint64_t m = (uint64_t)next_candidate;
        if (is_blum(m))
        {
            ensure_capacity(seq_len + 1, &seq, &seq_cap);
            seq[seq_len++] = (seq_t)m;
        }
        ++next_candidate;
    }

    return seq[n - 1];
}
