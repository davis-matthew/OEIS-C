/*
 * A267378  Primes of the form k^4 - k^2 + 7.
 * https://oeis.org/A267378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 7, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A267378 – primes of the form k^4 – k^2 + 7.
   a(n) = the nth prime value of k^4 – k^2 + 7 for k ≥ 0 (duplicates skipped). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    if (n == 0) {                     /* Off‑the‑offset calls return 0 */
        return 0;
    }

    static seq_t *prime_list = NULL;  /* growing array of primes found so far   */
    static size_t prime_len  = 0;     /* number of primes stored                */
    static size_t prime_cap  = 0;     /* capacity of prime_list
                                        (append_candidate handles growth) */

    static idx_t next_k      = 0;     /* next k to evaluate                     */
    static seq_t last_val    = 0;     /* value of previous k to skip duplicates  */
    static bool  has_last    = false;

    while (prime_len < (size_t)n) {
        seq_t kk   = (seq_t)next_k;       /* promote k to seq_t */
        seq_t ksq  = kk * kk;             /* k²                */
        seq_t val  = ksq * ksq - ksq + (seq_t)7;   /* k⁴ – k² + 7 */

        bool dup = has_last && val == last_val;
        if (!dup) {
            if (is_prime_big(val)) {
                append_candidate(&prime_list, &prime_len, &prime_cap, val);
            }
            last_val = val;
            has_last = true;
        }
        ++next_k;
    }

    return prime_list[(size_t)n - 1];
}
