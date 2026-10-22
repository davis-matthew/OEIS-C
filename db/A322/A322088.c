/*
 * A322088  Digits of one of the two 13-adic integers sqrt(3).
 * https://oeis.org/A322088
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

/* Sequence A322088: digits of one of the two 13‑adic integers sqrt(3).
 * The digit a(n) (0‑indexed) is the coefficient of 13ⁿ in the unique
 * 13‑adic integer x with x² ≡ 3 (mod 13ⁿ) and x ≡ 9 (mod 13).
 * The algorithm lifts the square root digit by digit using Hensel’s
 * lemma, maintaining the base‑13 expansion of the current approximation
 * and of its square.  The implementation works for any seq_t/idx_t
 * compatible with the build and requires only integer arithmetic.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static storage for the digits of the current approximation and its square */
static uint8_t *dig   = NULL;          /* a(n) for n = 0 .. computed-1   */
static uint8_t *sq    = NULL;          /* square of dig: digits 0 .. 2*computed-1 */
static size_t   dig_cap = 0;           /* allocated capacity for dig       */
static size_t   sq_cap  = 0;           /* allocated capacity for sq        */
static idx_t    computed = 0;          /* number of digits already computed */

/* allocate or grow storage to hold at least 'need' digits of dig
 * (and enough room for 2*need digits of sq).  Realloc handles the
 * void* cast needed for C++ compilers. */
static void ensure_cap(size_t need)
{
    if (need <= dig_cap) return;

    size_t new_cap = dig_cap ? dig_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;

    uint8_t *ndig = (uint8_t *)realloc(dig, new_cap * sizeof(uint8_t));
    uint8_t *nsq  = (uint8_t *)realloc(sq,  (2 * new_cap + 5) * sizeof(uint8_t));
    if (!ndig || !nsq) abort();

    /* zero newly allocated parts */
    memset(ndig + dig_cap, 0, (new_cap - dig_cap) * sizeof(uint8_t));
    memset(nsq  + sq_cap,  0, (2 * new_cap + 5 - sq_cap) * sizeof(uint8_t));

    dig    = ndig;
    sq     = nsq;
    dig_cap = new_cap;
    sq_cap  = 2 * new_cap + 5;
}

/* initialise the first digit and its square */
static void init(void)
{
    if (computed) return;
    ensure_cap(1);
    dig[0] = 9ul;        /* a(0) */
    sq[0]  = 3ul;        /* 9² = 81 = 3 + 6·13 */
    sq[1]  = 6ul;
    computed = 1;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    init();

    /* extend the precomputation up to the requested index */
    while (computed <= n) {
        size_t k = computed;              /* position of the new digit */
        ensure_cap(k + 1);                /* allocate space for it */

        /* compute the new digit a(k) */
        int sqk   = sq[k];                /* coefficient of 13ᵏ in current square */
        int r     = (13 - (sqk % 13)) % 13; /* -sqk (mod 13) */
        const int inv2a0 = 8;             /* inverse of 2·a(0)=18≡5 (mod 13) */
        int x      = (r * inv2a0) % 13;   /* new digit (0 ≤ x < 13) */
        dig[k] = (uint8_t)x;

        /* update the square: S'² = S² + 2·S·x·13ᵏ + x²·13²ᵏ  (mod 13ⁿ⁺¹) */
        /* 1. add 2·S·x·13ᵏ  */
        int carry = 0;
        size_t pos = k;
        for (size_t i = 0; i < k; ++i) {
            int prod = (int)dig[i] * 2 * x;          /* 0 ≤ prod ≤ 288  */
            int sum  = sq[pos] + prod + carry;
            sq[pos] = sum % 13;
            carry   = sum / 13;
            pos++;
        }
        while (carry) {
            int sum  = sq[pos] + carry;
            sq[pos] = sum % 13;
            carry   = sum / 13;
            pos++;
        }

        /* 2. add x²·13²ᵏ  (x² < 144) */
        int carry2 = x * x;
        pos = 2 * k;
        while (carry2) {
            int sum  = sq[pos] + carry2;
            sq[pos] = sum % 13;
            carry2  = sum / 13;
            pos++;
        }

        ++computed;   /* one more digit is now available */
    }

    return (seq_t)dig[n];
}
