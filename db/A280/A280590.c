/*
 * A280590  Numbers k such that for any positive integers (a, b), if a * b = k
 *          then sigma(a) + sigma(b) is a prime.
 * https://oeis.org/A280590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k such that for every pair of
 *            positive integers (a,b) with a*b = k, the sum sigma(a)+sigma(b)
 *            is prime. The implementation searches k from 1 upward, stores
 *            found values in a dynamic array, and reuses the array for
 *            subsequent calls. sigma(a) is the sum of all posit...
 * Range:     u8 n <= 27, u16 n <= 981, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t
sum_of_divisors_uint64(uint64_t n)
{
    uint64_t res = 1;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t term = 1;
            uint64_t pow = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pow *= p;
                term += pow;
            }
            res = (uint64_t)((unsigned __int128)res * term);
            if (res > UINT64_MAX) return res; /* overflow not expected in range */
        }
    }
    if (tmp > 1) {            /* remaining prime factor */
        res = (uint64_t)((unsigned __int128)res * (1 + tmp));
    }
    return res;
}

static bool
is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

static void
ensure_capacity(size_t need, size_t *cap, void **ptr)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 64 : *cap * 2;
    while (new_cap < need) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* The sequence function.  External linkage per requirement. */
seq_t a(idx_t n)
{
    static seq_t *found = NULL;   /* cache of found terms      */
    static size_t  found_cap = 0; /* cache capacity           */
    static size_t  found_len = 0; /* how many terms cached    */
    static uint64_t candidate = 1;/* next integer to test     */

    /* Grow cache as needed. */
    ensure_capacity(n, &found_cap, (void**)&found);

    while (found_len < n) {
        uint64_t k = candidate;
        bool ok = true;

        /* Check all divisor pairs (d, k/d). */
        for (uint64_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                uint64_t d1 = d;
                uint64_t d2 = k / d;
                uint64_t sigma_d1 = sum_of_divisors_uint64(d1);
                uint64_t sigma_d2 = sum_of_divisors_uint64(d2);
                uint64_t sum = sigma_d1 + sigma_d2;
                if (!is_prime_uint64(sum)) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            found[found_len++] = (seq_t)k;
        }
        ++candidate;
    }

    return found[n - 1];
}
