/*
 * A283317  Image of 0 under repeated applications of the morphism 0 ->
 *          0,0,0,0,1, 1 -> 1,1,1,1,0.
 * https://oeis.org/A283317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A283317: 0 → 00001, 1 → 11110
    The nth term (1‑based) is the symbol obtained by reading
    n‑1 in base 5 and following the substitution alphabetically.
    Start from symbol 0; for each digit d (0–4) do:
        if the current symbol is 0 and d==4 → 1; otherwise stay 0
        if the current symbol is 1 and d==4 → 0; otherwise stay 1
    The final symbol is a(n). */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t t = n - 1ULL;          /* 0‑based position */
    uint8_t digits[32];
    size_t len = 0;

    /* convert t to base 5, least significant digit first */
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 5ULL);
        t /= 5ULL;
    }

    seq_t s = 0; /* start symbol 0 */
    /* walk digits from most significant to least */
    for (size_t i = len; i > 0; --i) {
        uint8_t d = digits[i - 1];
        if (s == 0) {
            if (d == 4) s = 1;
        } else { /* s == 1 */
            if (d == 4) s = 0;
        }
    }
    return s;
}
