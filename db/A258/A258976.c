/*
 * A258976  Numbers n such that 1 + sigma(n)^2 is prime.
 * https://oeis.org/A258976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers n such that 1 + sigma(n)^2 is prime (OEIS A258976)
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 33, u64 n <= 1786,
 *            u128 n <= 1786, big any n
 * Helpers:   grow_seq, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *list   = NULL;      /* stored terms */
    static size_t len   = 0;          /* number of terms stored */
    static size_t cap   = 0;          /* allocation capacity */
    static idx_t last   = 0;          /* last n tested */

    if (n == 0) return 0;              /* offset is 1 */

    while (len < n) {
        ++last;                         /* next candidate */
        seq_t s   = sigma_of(last);     /* σ(n) */
        seq_t val = s * s + 1;          /* σ(n)^2 + 1 */
        if (is_prime_big(val))
        {
            grow_seq(&list, &cap, len + 1);   /* ensure space */
            list[len++] = (seq_t)last;        /* store candidate */
        }
    }
    return list[n - 1];
}
