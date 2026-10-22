/*
 * A322565  Digits of one of the two 17-adic integers sqrt(-2) that is related
 *          to A322563.
 * https://oeis.org/A322565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   normalize_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A322565: digits of one of the two 17‑adic integers sqrt(-2)
   that is related to A322563 (the other ends with digit 10).
   All values are base‑17 digits (0–16).  The recurrence used is
   a(0) = 7.  For n>0 let Sₙ = Σ_{i<n} a(i)·17ᶦ.
   The coefficient q of 17ⁿ in Sₙ²+2 is computed, and the next digit is
   a(n) = -(q * inv(2·a₀)) mod 17, where
   inv(2·a₀) ≡ 11 (mod 17).  The routine remembers computed digits
   and S², so queries at arbitrary indices take O(n) time in total. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/normalize_square.h"

/* --- lazy data --------------------------------------------- */
static uint8_t *digits   = NULL;   /* a(0)…a(cur-1)          */
static size_t  digits_len  = 0;    /* number of stored digits */
static size_t  digits_cap  = 0;    /* capacity of digits array */

static uint32_t *square = NULL;    /* S² in base 17, indices 0…   */
static size_t   sq_len   = 0;      /* current used length of square */
static size_t   sq_cap   = 0;      /* capacity of square array     */

/* --- helpers ------------------------------------------------- */
static void ensure_digits_cap(size_t need)
{
    if (need <= digits_cap) return;
    size_t newcap = digits_cap ? digits_cap : 16;
    while (newcap < need) newcap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(digits, newcap);
    if (!tmp) abort();
    for (size_t i = digits_cap; i < newcap; ++i) tmp[i] = 0;
    digits = tmp;
    digits_cap = newcap;
}

static void ensure_square_cap(size_t need)
{
    if (need <= sq_cap) return;
    size_t newcap = sq_cap ? sq_cap : 32;
    while (newcap < need) newcap <<= 1;
    uint32_t *tmp = (uint32_t*)realloc(square, newcap * sizeof(uint32_t));
    if (!tmp) abort();
    for (size_t i = sq_cap; i < newcap; ++i) tmp[i] = 0;
    square = tmp;
    sq_cap = newcap;
}

/* coefficient of 17^k in S²+2  */
static uint32_t compute_q(const uint32_t *sq, size_t k)
{
    uint32_t carry = 0;
    uint32_t val = (sq_len > 0 ? sq[0] : 0) + 2;
    uint32_t q = (k == 0) ? (val % 17) : 0;
    carry = val / 17;
    for (size_t i = 1; i <= k; ++i) {
        uint32_t v = (i < sq_len ? sq[i] : 0) + carry;
        if (i == k) q = v % 17;
        carry = v / 17;
    }
    return q;
}

static void initialise(void)
{
    if (digits_len) return;          /* already initialised */

    /* first digit a(0) = 7 */
    digits_cap = 16;
    digits = (uint8_t*)calloc(digits_cap, sizeof(uint8_t));
    digits[0] = 7;
    digits_len = 1;

    /* S = 7, so S² = 49 = 15 + 2*17  */
    sq_cap = 32;
    square = (uint32_t*)calloc(sq_cap, sizeof(uint32_t));
    square[0] = 15;
    square[1] = 2;
    sq_len = 2;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    initialise();

    /* extend computation up to n if needed */
    while (digits_len <= (size_t)n) {
        size_t k = digits_len;  /* new digit index (current max + 1) */

        /* compute a(k) */
        uint32_t q = compute_q(square, k);
        /* inverse of 2*a0 = 14 (mod 17) is 11 */
        uint32_t ak = (17 - (q * 11U) % 17U) % 17U;

        /* store digit */
        ensure_digits_cap(k + 1);
        digits[k] = (uint8_t)ak;
        digits_len = k + 1;

        /* update square S² to include new digit */
        size_t need_sq = 2 * k + 3;   /* room for indices up to 2k */
        ensure_square_cap(need_sq);

        /* self term */
        square[2 * k] += (uint32_t)ak * (uint32_t)ak;

        /* cross terms: 2*a(k)*S_prev shifted by k */
        for (size_t i = 0; i < k; ++i) {
            uint32_t add = 2U * (uint32_t)ak * (uint32_t)digits[i];
            square[i + k] += add;
        }

        /* normalize up to the new maximum possible index */
        size_t new_len = 2 * k + 1;
        if (new_len < sq_len) new_len = sq_len;
        normalize_square(square, &new_len);
        sq_len = new_len;
    }

    return (seq_t)digits[n];
}
