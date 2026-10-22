/*
 * A378186  With p(n) = A002145(n) = n-th non-Pythagorean prime, a(n) is the
 *          least k such p(n) + k is a non-Pythagorean prime and 2 p(n) + k -
 *          5 is a Pythagorean prime; and a(n) = 0 if there is no such k .
 * https://oeis.org/A378186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A378186: a(n) is the least positive k such that
   p(n)+k is a non‑Pythagorean prime (≡3 mod 4) and
   2 p(n)+k−5 is a Pythagorean prime (≡1 mod 4).
   p(n) is the n-th prime ≡3 mod 4.
   For each n ≥ 1, a(n) = 0 if no such k exists. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n);        */
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t n);  */
#include "../utils/seq_realloc.h"    /* seq_t *seq_realloc(seq_t *,size_t,size_t); */

static size_t np_len = 0;              /* number of stored non‑Pythagorean primes   */
static size_t np_cap = 0;              /* capacity of np_arr                         */
static seq_t *np_arr = NULL;           /* array of non‑Pythagorean primes           */

/* Grow the array to hold at least (np_len + need) elements. */
static void ensure_capacity_np(size_t need)
{
    if (np_cap >= np_len + need) return;
    size_t cap = np_cap ? np_cap << 1 : 64;
    while (cap < np_len + need) cap <<= 1;
    np_arr = seq_realloc(np_arr, np_cap, cap);
    np_cap = cap;
}

/* Populate np_arr until it contains at least n non‑Pythagorean primes. */
static void ensure_np_primes(idx_t n)
{
    if (np_len >= n) return;

    uint64_t last = (np_len == 0) ? 1 : (uint64_t)np_arr[np_len - 1];
    while ((idx_t)np_len < n) {
        last = next_prime(last);           /* next prime > last */
        if (last % 4 == 3) {                /* non‑Pythagorean prime */
            ensure_capacity_np(1);
            np_arr[np_len++] = (seq_t)last;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset starts at 1 */

    /* Ensure p(n) is available */
    ensure_np_primes(n);
    seq_t p = np_arr[n - 1];                /* nth non‑Pythagorean prime */

    const seq_t two_p = p + p;              /* 2·p */

    /* Search k in multiples of 4, starting from 4 to enforce p+k>p. */
    seq_t k = 4;
    while (true) {
        seq_t q1 = p + k;                  /* candidate non‑Pythagorean prime */
        if (is_prime_big(q1)) {
            seq_t t = two_p + k;           /* 2p + k */
            if (t >= 5) {                  /* avoid underflow when subtracting 5 */
                seq_t q2 = t - 5;          /* candidate Pythagorean prime */
                if (is_prime_big(q2))
                    return k;
            }
        }
        k += 4;
    }
}
