/*
 * A246725  Decimal expansion of r_3, the third smallest radius for which a
 *          compact packing of the plane exists, with disks of radius 1 and
 *          r_3.
 * https://oeis.org/A246725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1)*(sqrt(17)-3)/4)) mod 10 The digits are
 *            obtained by the classic non‑repeating square root extraction
 *            algorithm for sqrt(17). All intermediate magnitudes are handled
 *            with an elementary dynamic base‑1e9 big‑integer implementation
 *            that is small enough to run under the time limi...
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Simple unbounded integer (base 1e9)                                 */
/* ------------------------------------------------------------------ */
typedef struct {
    size_t len;       /* number of used digits (least significant first) */
    size_t cap;       /* allocated capacity */
    uint32_t *d;      /* array of digits, base 1e9 */
} BigInt;

#define BIGINT_BASE 1000000000U

/* Ensure capacity */
static void bigint_ensure(BigInt *b, size_t need)
{
    if (b->cap >= need) return;
    size_t newcap = b->cap ? b->cap * 2 : 4;
    if (newcap < need) newcap = need;
    uint32_t *newd = (uint32_t *)realloc(b->d, newcap * sizeof(uint32_t));
    if (!newd) abort();
    b->d = newd;
    b->cap = newcap;
}

/* Initialise an empty big integer with value zero */
static void bigint_init(BigInt *b)
{
    b->len = 1;
    b->cap = 4;
    b->d = (uint32_t *)malloc(b->cap * sizeof(uint32_t));
    if (!b->d) abort();
    b->d[0] = 0;
}

/* Free a big integer */
static void bigint_free(BigInt *b)
{
    free(b->d);
}

/* Set b to an unsigned 32‑bit value */
static void bigint_set_uint(BigInt *b, uint32_t v)
{
    bigint_ensure(b, 1);
    b->len = 1;
    b->d[0] = v;
}

/* Copy src into dst (assumes dst has enough capacity) */
static void bigint_copy(BigInt *dst, const BigInt *src)
{
    bigint_ensure(dst, src->len);
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
    dst->len = src->len;
}

/* Add a small unsigned integer to big integer b (in place) */
static void bigint_add_small(BigInt *b, uint32_t add)
{
    uint64_t carry = add;
    size_t i = 0;
    while (carry && i < b->len) {
        uint64_t sum = (uint64_t)b->d[i] + carry;
        b->d[i] = (uint32_t)(sum % BIGINT_BASE);
        carry = sum / BIGINT_BASE;
        ++i;
    }
    if (carry) {
        bigint_ensure(b, b->len + 1);
        b->d[b->len++] = (uint32_t)carry;
    }
}

/* Multiply big integer b by a small unsigned integer (in place) */
static void bigint_mul_small(BigInt *b, uint32_t mul)
{
    if (mul == 0) {
        b->len = 1;
        b->d[0] = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * mul + carry;
        b->d[i] = (uint32_t)(prod % BIGINT_BASE);
        carry = prod / BIGINT_BASE;
    }
    while (carry) {
        bigint_ensure(b, b->len + 1);
        b->d[b->len++] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
    }
}

/* Subtract b from a (a >= b), result in a (in place) */
static void bigint_sub_inplace(BigInt *a, const BigInt *b)
{
    int64_t borrow = 0;
    size_t i = 0;
    for (; i < a->len; ++i) {
        int64_t ai = a->d[i];
        int64_t bi = (i < b->len) ? b->d[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) {
            diff += BIGINT_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)diff;
    }
    /* Trim leading zeros (most significant) */
    while (a->len > 1 && a->d[a->len - 1] == 0) a->len--;
}

/* Compare a and b: return 1 if a>b, 0 if equal, -1 if a<b */
static int bigint_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

/* Compute big integer modulo a small unsigned integer m */
static uint32_t bigint_mod_small(const BigInt *b, uint32_t m)
{
    uint32_t rem = 0;
    for (size_t i = b->len; i-- > 0;) {
        uint64_t tmp = (uint64_t)rem * BIGINT_BASE + b->d[i];
        rem = (uint32_t)(tmp % m);
    }
    return rem;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* maximal digit index */
    size_t m_needed = (size_t)n + 1;      /* we need sqrt to this many digits */

    /* Initialise big integers */
    BigInt p, r, temp, temp2, cand;
    bigint_init(&p); bigint_set_uint(&p, 4);   /* floor(sqrt(17)) = 4 */
    bigint_init(&r); bigint_set_uint(&r, 1);   /* remainder = 17 - 4^2 = 1 */
    bigint_init(&temp); bigint_init(&temp2);
    bigint_init(&cand);

    /* Loop to compute m_needed digits of sqrt(17) */
    for (size_t step = 0; step < m_needed; ++step) {
        /* r = r * 100 */
        bigint_mul_small(&r, 100);

        /* temp = 20 * p */
        bigint_copy(&temp, &p);
        bigint_mul_small(&temp, 20);

        int d_chosen = 0;
        for (int d = 9; d >= 0; --d) {
            /* temp2 = temp + d */
            bigint_copy(&temp2, &temp);
            bigint_add_small(&temp2, (uint32_t)d);

            /* cand = temp2 * d */
            bigint_copy(&cand, &temp2);
            bigint_mul_small(&cand, (uint32_t)d);

            if (bigint_cmp(&cand, &r) <= 0) {
                d_chosen = d;
                /* r = r - cand */
                bigint_sub_inplace(&r, &cand);
                break;
            }
        }
        /* p = p * 10 + d_chosen */
        bigint_mul_small(&p, 10);
        bigint_add_small(&p, (uint32_t)d_chosen);
    }

    /* Compute remainders needed for digit extraction */
    uint32_t p_mod4   = bigint_mod_small(&p, 4);
    uint32_t p_mod40  = bigint_mod_small(&p, 40);

    /* Compute 10^m_needed modulo 4 and 40 efficiently */
    uint32_t pow4  = 1 % 4;
    uint32_t pow40 = 1 % 40;
    for (size_t i = 0; i < m_needed; ++i) {
        pow4  = (pow4 * 10) % 4;
        pow40 = (pow40 * 10) % 40;
    }

    /* t = p - 3*10^m_needed   (mod 4 and mod 40) */
    uint32_t t_mod4   = (p_mod4 + 4 - (3 * pow4) % 4) % 4;
    uint32_t t_mod40  = (p_mod40 + 40 - (3 * pow40) % 40) % 40;

    /* a(n) = (t_mod40 - t_mod4) / 4  (guaranteed divisible by 4) */
    uint32_t digit = (t_mod40 - t_mod4) / 4;

    /* Free big integers */
    bigint_free(&p);
    bigint_free(&r);
    bigint_free(&temp);
    bigint_free(&temp2);
    bigint_free(&cand);

    return (seq_t)digit;
}
