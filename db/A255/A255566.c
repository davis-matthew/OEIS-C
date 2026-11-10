/*
 * A255566  a(0) = 0; after which, a(2n) = A255411(a(n)), a(2n+1) =
 *          A256450(a(n)).
 * https://oeis.org/A255566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(0) = 0; a(2n) = A255411(a(n)), a(2n+1) = A256450(a(n));
 *            computed by walking the bits of n from the top, applying A255411
 *            for a 0 bit and A256450 for a 1 bit
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: standard, pass, 88 terms through n = 1000 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A255411_a(seq_t n)
{
    unsigned char dig[64];
    unsigned int k = 0;
    seq_t rem = n;
    while (rem > 0) {
        unsigned int div = k + 2;
        dig[k] = (unsigned char)(rem % div);
        rem /= div;
        ++k;
    }
    seq_t result = 0;
    seq_t fact = 1;
    for (unsigned int i = 1; i <= k + 1; ++i) {
        seq_t digit = 0;
        if (i > 1 && dig[i - 2] != 0)
            digit = (seq_t)dig[i - 2] + 1;
        result += digit * fact;
        fact *= (seq_t)(i + 1);
    }
    return result;
}

/* count of integers in [0,x] with no digit 1 in factorial base */
static seq_t A256450_count_no_one(seq_t x)
{
    unsigned char dig[64];
    unsigned int k = 0;
    seq_t rem = x;
    while (rem > 0) {
        unsigned int div = k + 2;
        dig[k] = (unsigned char)(rem % div);
        rem /= div;
        ++k;
    }
    seq_t fact[65];
    fact[0] = 1;
    for (unsigned int i = 1; i <= k; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    seq_t total = 0;
    bool intact = true;
    for (unsigned int ii = k; ii > 0; --ii) {
        unsigned int i = ii - 1;
        unsigned int d = dig[i];
        unsigned int c = (d >= 2) ? d - 1 : d;
        total += (seq_t)c * fact[i];
        if (d == 1) { intact = false; break; }
    }
    if (intact)
        total += 1;
    return total;
}

/* m-th (0-indexed) number with a digit 1 in factorial base */
static seq_t A256450_a(seq_t m)
{
    seq_t target = m + 1;
    seq_t x = m + 1;
    for (;;) {
        seq_t g = x + 1 - A256450_count_no_one(x);
        if (g >= target)
            break;
        x += target - g;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t v = 0;
    if (n == 0)
        return v;
    int top = 63;
    while (!((n >> top) & 1))
        --top;
    for (int b = top; b >= 0; --b) {
        if ((n >> b) & 1)
            v = A256450_a(v);
        else
            v = A255411_a(v);
    }
    return v;
}
