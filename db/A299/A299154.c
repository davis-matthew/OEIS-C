/*
 * A299154  a(n) is the distance from prime(n) to the next prime that has the
 *          same digital root, or a(n) = 0 if there is no greater prime with
 *          the same digital root.
 * https://oeis.org/A299154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = distance from prime(n) to the next prime that has the
 *            same digital * root; if prime(n) = 3 (the only prime with no
 *            larger same‑digital‑root prime) the * value is 0. This function
 *            uses the utility get_prime(k) to obtain the k‑th prime * and
 *            is_prime_big(v) to test primality of 64‑bit candid...
 * Range:     u8 n <= 45, u16 n <= 5437, u32 n <= 6071, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n==0, return 0 */
        return (seq_t)0;

    /* prime(n) – get_prime is 1‑based */
    uint64_t p = (uint64_t)get_prime(n);

    /* special case: only prime that never has a larger same‑digital‑root prime */
    if (p == 3u)
        return (seq_t)0;

    /* search the next prime with the same digital root by stepping in increments of 9 */
    uint64_t candidate = p + 9u;
    while (!is_prime_big((seq_t)candidate))
        candidate += 9u;

    return (seq_t)(candidate - p);
}
