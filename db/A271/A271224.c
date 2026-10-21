/*
 * A271224  Digits of one of the two 3-adic integers sqrt(-2). Here the
 *          sequence with first digit 2.
 * https://oeis.org/A271224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------
   Sequence A271224 – digits of one of the two 3‑adic integers √(−2)
   in base 3.  The nth digit satisfies the congruence

     (x + a(n)·3ⁿ)² ≡ −2   (mod 3ⁿ⁺¹)

   where x = Σ_{k=0}^{n−1} a(k)·3ᵏ.  a(0) = 2 and
   for n ≥ 1 the unique digit a(n) ∈ {0,1,2} is obtained by
   the Newton‑type update described below.  The implementation
   keeps the partial sum x and the partial square x² in base 3
   and updates them digit by digit, which yields an O(n) algorithm.
------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------
   Global dynamic arrays storing the base‑3 digits of the partial sum
   `x` and the partial square `x²` (in least‑significant‑order).
------------------------------------------------------------------- */
static unsigned char *g_digits = (unsigned char*)NULL;
static size_t g_digits_len = 0;
static size_t g_digits_cap = 0;

static uint32_t *g_prod   = (uint32_t*)NULL;
static size_t g_prod_len   = 0;
static size_t g_prod_cap   = 0;

static int      g_initialised = 0;

/* ------------------------------------------------------------------
   Ensure the digit array has at least `needed` capacity.
------------------------------------------------------------------- */
static void ensure_digits_cap(size_t needed)
{
    if (needed <= g_digits_cap) return;
    size_t newcap = g_digits_cap ? g_digits_cap << 1 : 16;
    if (newcap < needed) newcap = needed;
    unsigned char *p = (unsigned char*)realloc(g_digits, newcap);
    if (!p) abort();
    g_digits = p;
    g_digits_cap = newcap;
}

/* ------------------------------------------------------------------
   Ensure the product array has at least `needed` capacity.
------------------------------------------------------------------- */
static void ensure_prod_cap(size_t needed)
{
    if (needed <= g_prod_cap) return;
    size_t newcap = g_prod_cap ? g_prod_cap << 1 : 32;
    if (newcap < needed) newcap = needed;
    uint32_t *p = (uint32_t*)realloc(g_prod, newcap * sizeof(uint32_t));
    if (!p) abort();
    g_prod = p;
    g_prod_cap = newcap;
}

/* ------------------------------------------------------------------
   Append a new base‑3 digit `d` (0 ≤ d ≤ 2) to the current number.
   The arrays `g_digits` and `g_prod` are updated to represent the
   new partial sum `x` and its square `x²` (both modulo a suitable
   power of 3).
------------------------------------------------------------------- */
static void add_digit(uint8_t d)
{
    size_t m = g_digits_len;                  /* current number of digits */

    /* Ensure space for the new digit and for the new square.          */
    size_t new_need = 2 * m + 1;
    size_t old_len  = g_prod_len;
    ensure_prod_cap(new_need);
    if (g_prod_len < new_need) {
        for (size_t i = old_len; i < new_need; ++i) g_prod[i] = 0;
        g_prod_len = new_need;
    }

    /* Update g_prod to account for the new digit:                  */
    /*   (x + d·3ᵐ)² = x² + 2·d·x·3ᵐ + d²·3²ᵐ                                 */
    for (size_t i = 0; i < m; ++i)
        g_prod[m + i] += 2u * d * g_digits[i];
    g_prod[2 * m] += (uint32_t)d * d;

    /* Normalise from position m upwards (carry propagation).        */
    for (size_t i = m; i < g_prod_len; ++i) {
        uint32_t carry = g_prod[i] / 3u;
        g_prod[i] %= 3u;
        if (carry) {
            if (i + 1 == g_prod_len) {
                ++g_prod_len;
                ensure_prod_cap(g_prod_len);
                g_prod[i + 1] = carry;
            } else {
                g_prod[i + 1] += carry;
            }
        }
    }

    /* Store the new digit. */
    ensure_digits_cap(g_digits_len + 1);
    g_digits[g_digits_len++] = d;
}

/* ------------------------------------------------------------------
   Compute all digits up to index `n` (inclusive).  The arrays are
   lazily initialised on the first call and then expanded as needed.
------------------------------------------------------------------- */
static void ensure_up_to(idx_t n)
{
    if (!g_initialised) {
        /* initialise capacities, but length stays zero. */
        ensure_digits_cap(16);
        ensure_prod_cap(32);
        g_initialised = 1;
    }

    /* Make sure the first digit is present. */
    if (g_digits_len == 0) {
        add_digit(2);                      /* a(0) = 2 */
    }

    /* Compute missing digits. */
    while (g_digits_len <= (size_t)n) {
        size_t k = g_digits_len;              /* index to compute */

        /* Compute r = coefficient of 3ᵏ in (x² + 2).          */
        uint32_t carry = 0;
        for (size_t i = 0; i < k; ++i) {
            uint32_t sum = g_prod[i] + ((i == 0) ? 2u : 0u) + carry;
            carry = sum / 3u;
        }
        uint32_t r = (g_prod[k] + carry) % 3u;      /* (x²+2)/3ᵏ  mod 3 */

        /* a(k) satisfies 2·x·a(k) ≡ -r (mod 3).
           For n ≥ 1, 2·x ≡ 1 (mod 3), so a(k) ≡ (3‑r) mod 3.   */
        uint32_t d = (3u - r) % 3u;                  /* digit 0/1/2 */

        add_digit((uint8_t)d);
    }
}

/* ------------------------------------------------------------------
   Sequence entry function requested by the harness.
------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return (seq_t)g_digits[(size_t)n];
}
