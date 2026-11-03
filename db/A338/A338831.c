/*
 * A338831  a(n) is the greatest number not yet in the sequence with the same
 *          number of digits and the same smallest digit as n.
 * https://oeis.org/A338831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the greatest unused number that has the same decimal *
 *            length and the same smallest digit as n. The sequence is a *
 *            self‑inverse permutation of the nonnegative integers. The *
 *            implementation computes, for each input n, the rank of n inside
 *            * the group of numbers with equal (length, smalle...
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 84 terms through n = 9424 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Small integer power (base^exp) – base ≤ 10, exp small */
static unsigned long long pow_ull(unsigned base, idx_t exp)
{
    unsigned long long r = 1;
    while (exp--) r *= base;
    return r;
}

/* Count of d‑digit numbers whose minimum digit is ≥ t.
 * For d==1 special handling allows digit 0.
 * Returns 0 when t>9. */
static idx_t count_min_geq(idx_t t, idx_t d)
{
    if (t > 9) return 0;
    if (d == 1) {
        /* Leading zero only allowed when d==1 */
        return (idx_t)(10 - t);               /* 10-t possibilities */
    }
    unsigned long long first, second;
    if (t == 0) {
        first  = 9;            /* leading digit 1–9 */
        second = 10;           /* remaining digits 0–9 */
    } else {
        first  = (unsigned long long)(10 - t);   /* digits t–9 */
        second = (unsigned long long)(10 - t);
    }
    unsigned long long powrem = pow_ull((unsigned)second, d - 1);
    return (idx_t)(first * powrem);
}

/* Size of the group of d‑digit numbers whose minimal digit equals s. */
static idx_t bucket_size(idx_t d, unsigned char s)
{
    return count_min_geq(s, d) - count_min_geq(s + 1, d);
}

/* Rank of n (0‑based) within its group.  digits[] contains the decimal
 * digits of n in least‑significant first order. */
static idx_t compute_rank(idx_t n, idx_t d, unsigned char s,
                          const uint8_t *digits)
{
    idx_t rank = 0;
    bool seen_s = false;

    for (idx_t pos = 0; pos < d; ++pos) {
        int msd_index = (int)(d - 1 - pos);
        int nd = digits[msd_index];               /* current digit */

        int low = (pos == 0 && d > 1) ? ((s > 0) ? s : 1) : s;

        for (int c = low; c < nd; ++c) {
            bool seen_s_next = seen_s || (c == s);
            idx_t rem = d - 1 - pos;
            idx_t count;

            if (rem == 0) {
                count = seen_s_next ? 1 : 0;
            } else {
                unsigned long long tot = pow_ull(10 - s, rem);
                if (seen_s_next) {
                    count = (idx_t)tot;
                } else {
                    unsigned long long sub = pow_ull(9 - s, rem);
                    count = (idx_t)(tot - sub);
                }
            }
            rank += count;
        }

        if (nd == s) seen_s = true;
    }
    return rank;
}

/* Return the number that occupies the given rank (0‑based) within
 * the group of d‑digit numbers whose minimal digit equals s.  digits
 * are constructed from most significant to least significant. */
static seq_t get_value(idx_t d, unsigned char s, idx_t rank)
{
    seq_t value = 0;
    bool seen_s = false;

    for (idx_t pos = 0; pos < d; ++pos) {
        int low = (pos == 0 && d > 1) ? ((s > 0) ? s : 1) : s;
        idx_t rem = d - 1 - pos;

        for (int c = low; c <= 9; ++c) {
            bool seen_s_next = seen_s || (c == s);
            idx_t count;

            if (rem == 0) {
                count = seen_s_next ? 1 : 0;
            } else {
                unsigned long long tot = pow_ull(10 - s, rem);
                if (seen_s_next) {
                    count = (idx_t)tot;
                } else {
                    unsigned long long sub = pow_ull(9 - s, rem);
                    count = (idx_t)(tot - sub);
                }
            }

            if (rank >= count) {
                rank -= count;
            } else {
                value = value * 10 + c;
                seen_s = seen_s_next;
                break;
            }
        }
    }
    return value;
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Extract decimal digits of n (least significant first). */
    uint8_t digits[64];
    idx_t d = 0;
    seq_t x = (seq_t)n;

    if (x == 0) {
        digits[d++] = 0;
    } else {
        while (x > 0) {
            digits[d++] = (uint8_t)(x % 10);
            x /= 10;
        }
    }

    /* One‑digit numbers map to themselves. */
    if (d == 1) return (seq_t)n;

    /* Smallest digit in n. */
    unsigned char s = 9;
    for (idx_t i = 0; i < d; ++i) {
        if (digits[i] < s) s = digits[i];
    }

    /* Size of the group. */
    idx_t sz = bucket_size(d, s);

    /* Rank of n within its group. */
    idx_t rnk = compute_rank(n, d, s, digits);

    /* Complementary rank for the paired number. */
    idx_t target = sz - 1 - rnk;

    /* Construct the number with that rank. */
    return get_value(d, s, target);
}
