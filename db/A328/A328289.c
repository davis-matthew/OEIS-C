/*
 * A328289  a(n) is the number of positive numbers < n that have no digit in
 *          common with n.
 * https://oeis.org/A328289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{ m : 1 ≤ m < n, digit_mask(m) ∩ digit_mask(n) = ∅ }|
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 10‑bit bitmask of the digits present in a number. */
static uint16_t *digit_masks = NULL;
/* Current allocated size (number of entries that are valid). */
static size_t masks_cap = 0;

/* Make sure that digit_masks[0 .. n] are computed. */
static void ensure_masks(idx_t n)
{
    if ((size_t)n < masks_cap)
        return;

    size_t new_cap = masks_cap > 0 ? masks_cap << 1 : 128;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    void *p = realloc(digit_masks, new_cap * sizeof(uint16_t));
    if (!p)
        abort();                 /* allocation failed */
    digit_masks = (uint16_t *)p;

    for (size_t i = masks_cap; i < new_cap; ++i) {
        uint16_t mask = 0;
        idx_t t = (idx_t)i;
        if (t == 0) {
            /* mask stays 0 for number 0 */
        } else {
            while (t > 0) {
                mask |= 1u << (t % 10);
                t /= 10;
            }
        }
        digit_masks[i] = mask;
    }
    masks_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    ensure_masks(n);

    uint16_t mask_n = digit_masks[(size_t)n];
    seq_t cnt = 0;

    for (idx_t k = 1; k < n; ++k) {
        if ((digit_masks[(size_t)k] & mask_n) == 0)
            ++cnt;
    }
    return cnt;
}
