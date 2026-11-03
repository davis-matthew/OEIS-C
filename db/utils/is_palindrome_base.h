/* is_palindrome_base: whether n is a base-b palindrome.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PALINDROME_BASE_H
#define OEISC_UTILS_IS_PALINDROME_BASE_H
#include <stdbool.h>
#include <stdint.h>

/* Return non‑negative palindrome status of x in base 'base' (base >= 2) */
static bool is_palindrome_base(seq_t x, idx_t base)
{
    /* At most a few dozen digits for all n <= 10000 and k <= 20 */
    unsigned digits[128];
    int len = 0;
    seq_t tmp = x;
    seq_t sb = (seq_t)base;

    /* Extract digits least significant first */
    while (tmp != 0) {
        digits[len++] = (unsigned)(tmp % sb);
        tmp /= sb;
    }

    /* Check symmetry */
    for (int i = 0; i < len / 2; ++i) {
        if (digits[i] != digits[len - 1 - i])
            return false;
    }
    return true;
}

#endif
