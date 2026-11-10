/*
 * A377387  a(n) is the least number k such that A377384(k) = n, or -1 if no
 *          such number exists.
 * https://oeis.org/A377387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = least k with A377384(k) = n, found by a backward search
 *            (iterative bound doubling) over chains x -> x*s where s =
 *            A034968(x*s); chains start at factorials m! (n steps) or small
 *            non-Niven seeds (n-1 steps)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A377387_best;
static uint64_t A377387_fact[21];
static int A377387_init = 0;

static void A377387_setup(void)
{
    if (A377387_init) return;
    A377387_fact[0] = 1;
    for (int i = 1; i <= 20; i++)
        A377387_fact[i] = A377387_fact[i - 1] * (uint64_t)i;
    A377387_init = 1;
}

/* max possible factorial-base digit sum of any number <= lim */
static unsigned A377387_smax(uint64_t lim)
{
    unsigned m = 1;
    while (m < 20 && A377387_fact[m + 1] <= lim) m++;
    return m * (m + 1) / 2;
}

/* true iff factorial-base digit sum of x equals s */
static bool A377387_ds_eq(uint64_t x, uint64_t s)
{
    uint64_t sum = 0, i = 2;
    while (x) {
        sum += x % i;
        if (sum > s) return false;
        x /= i;
        i++;
    }
    return sum == s;
}

static uint64_t A377387_ds(uint64_t x)
{
    uint64_t sum = 0, i = 2;
    while (x) {
        sum += x % i;
        x /= i;
        i++;
    }
    return sum;
}

static void A377387_dfs(uint64_t v, int rem)
{
    if (rem == 0) {
        if (v < A377387_best) A377387_best = v;
        return;
    }
    uint64_t lim = (A377387_best - 1) >> (rem - 1);
    if (v == 0 || v > lim) return;
    uint64_t smx = lim / v;
    uint64_t cap = A377387_smax(lim);
    if (smx > cap) smx = cap;
    for (uint64_t s = 2; s <= smx; s++) {
        lim = (A377387_best - 1) >> (rem - 1);
        if (v * s > lim) break;
        uint64_t x = v * s;
        if (A377387_ds_eq(x, s))
            A377387_dfs(x, rem - 1);
    }
}

seq_t a(idx_t n)
{
    static uint64_t cache[64];
    static bool have[64];
    const uint64_t YCAP = 30000;

    if (n > 60) return (seq_t)0 - (seq_t)1;
    if (have[n]) return (seq_t)cache[n];
    A377387_setup();

    int nn = (int)n;
    uint64_t B = (uint64_t)1 << 12;
    uint64_t result = 0;
    bool found = false;

    while (B < ((uint64_t)1 << 62)) {
        A377387_best = B + 1;

        /* case A: chains ending at a factorial after nn steps */
        for (int m = 1; m <= 20; m++) {
            uint64_t f = A377387_fact[m];
            if (nn > 0 && f > ((A377387_best - 1) >> nn)) break;
            A377387_dfs(f, nn);
        }

        /* case B: chains ending at a non-Niven, nonfactorial seed */
        if (nn >= 1) {
            for (uint64_t y = 2; y <= YCAP; y++) {
                if (y > ((A377387_best - 1) >> (nn - 1))) break;
                uint64_t d = A377387_ds(y);
                if (d == 1) continue;
                if (y % d == 0) continue;
                A377387_dfs(y, nn - 1);
            }
        }

        if (A377387_best <= B) {
            result = A377387_best;
            found = true;
            break;
        }
        B <<= 2;
    }

    if (!found) return (seq_t)0 - (seq_t)1;
    cache[n] = result;
    have[n] = true;
    return (seq_t)result;
}
