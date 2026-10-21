/*
 * A254159  Decimal expansion of atomic mass constant energy equivalent in J.
 * https://oeis.org/A254159
 *
 * Offset:    -9    (a(-9) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the (n+19)-th decimal digit of the constant
 *            1.492418085597800936052020064 × 10⁻¹⁰ (i.e. the decimal
 *            expansion of (166053906660 × 299792458²) / 10³⁸)
 * Range:     u8 n <= -1, u16 n <= -1, u32 n <= -1, u64 n <= -1, u128 n <= -1,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Position after the decimal point (1‑based).
       Digits 1‑9 are leading zeros. */
    const idx_t pos = n + 19;
    if (pos <= 9) return (seq_t)0;

    /* Precompute the 29 non‑zero digits of the numerator only once. */
    static bool init = false;
    static uint8_t digits[29];     /* digits[0] = most significant */

    if (!init)
    {
        /* Numerator N = 166053906660 × 299792458² */
        const unsigned long long a = 166053906660ULL;
        const unsigned long long b = 89875517873681764ULL;   /* 299792458² */
        unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;

        /* Extract digits (least significant first) */
        unsigned __int128 tmp = prod;
        for (int i = 28; i >= 0; --i)
        {
            digits[i] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        init = true;
    }

    if (pos <= 38)
        return (seq_t)digits[pos - 10];

    /* After position 38 all digits are zero (the expansion terminates). */
    return (seq_t)0;
}
