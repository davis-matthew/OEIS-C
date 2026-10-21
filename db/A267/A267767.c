/*
 * A267767  Numbers whose base-7 representation is a square when read in base
 *          10.
 * https://oeis.org/A267767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 183, u32 n <= 5407, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers whose base-7 representation is a square when read in base 10.
    The terms are obtained by iterating over all perfect squares m².
    If all decimal digits of m² are < 7 then the same digit string,
    interpreted as a base‑7 number, is a term of the sequence.
*/

#include <stdlib.h>
#include <stdbool.h>

/* helper to grow the cache array if needed */
static void grow_cache(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap >= needed)
        return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t m_next      = 0;   /* next m to process (starting from 0) */

    if (n == 0)  /* undefined in OEIS (offset 1), but guard just in case */
        return 0;

    /* Ensure the cache contains at least n terms */
    while (cache_size < (size_t)n) {
        seq_t s = (seq_t)m_next * (seq_t)m_next;   /* m² */

        /* process square s */
        seq_t value = 0;
        seq_t weight = 1;
        bool valid = true;

        if (s == 0) {
            /* special case: square 0 */
            value = 0;
        } else {
            seq_t temp = s;
            while (temp > 0) {
                seq_t digit = temp % 10;
                if (digit >= 7) {           /* invalid digit for base‑7 */
                    valid = false;
                    break;
                }
                value += digit * weight;
                weight *= 7;
                temp /= 10;
            }
        }

        if (valid) {
            grow_cache(cache_size + 1, &cache_cap, &cache);
            cache[cache_size++] = value;
        }

        ++m_next;
    }

    return cache[n - 1];
}
