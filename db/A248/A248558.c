/*
 * A248558  Squares of the digits of the decimal expansion of e.
 * https://oeis.org/A248558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (digit of e at position n)² (n ≥ 1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth square of the nth decimal digit of e.   */
/* This implementation precomputes e to a fixed precision (10000 digits). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Fixed precision for e: number of decimal digits after the integer part.  */
#define PRECISION_DIGITS 10000
/* Base 10^9 is used for internal big‑integer representation.            */
#define BASE 1000000000U
/* Number of decimal digits represented by one limb.                       */
#define LIMB_DIGITS 9
/* Maximum number of limbs needed to hold PRECISION_DIGITS+1 decimal digits. */
#define MAX_LIMBS ((PRECISION_DIGITS / LIMB_DIGITS) + 5)

/* Pre‑computed 10^k for k = 0 .. 9 */
static const uint32_t pow10[10] = {
    1u, 10u, 100u, 1000u, 10000u, 100000u,
    1000000u, 10000000u, 100000000u, 1000000000u
};

/* Structure for big integers in base BASE.  */
typedef struct {
    uint32_t d[MAX_LIMBS];
    size_t len;  /* number of used limbs (most significant limb may be zero) */
} BigInt;

/* Remove leading zero limbs. */
static void big_trim(BigInt *a) {
    while (a->len > 0 && a->d[a->len - 1] == 0u) {
        a->len--;
    }
}

/* Initialise BigInt to 10^exp (exp ≥ 0). */
static void big_pow10(BigInt *a, int exp) {
    for (int i = 0; i < MAX_LIMBS; i++) a->d[i] = 0u;
    a->len = exp / LIMB_DIGITS + 1;
    int rem = exp % LIMB_DIGITS;
    a->d[exp / LIMB_DIGITS] = pow10[rem];
    /* other limbs are zero */
    if (rem == 0) {
        /* ensure that 10^0 = 1 when exp == 0 */
        if (exp == 0) a->d[0] = 1u;
    }
    big_trim(a);
}

/* Adds b to a: a += b.  Both a and b must be valid BigInt. */
static void big_add(BigInt *a, const BigInt *b) {
    size_t maxlen = a->len > b->len ? a->len : b->len;
    uint64_t carry = 0;
    for (size_t i = 0; i < maxlen; i++) {
        uint64_t av = (i < a->len) ? a->d[i] : 0u;
        uint64_t bv = (i < b->len) ? b->d[i] : 0u;
        uint64_t sum = av + bv + carry;
        if (i < MAX_LIMBS) a->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
    }
    if (carry && maxlen < MAX_LIMBS) {
        a->d[maxlen] = (uint32_t)carry;
        maxlen++;
    }
    a->len = maxlen;
    big_trim(a);
}

/* Divides a by small integer d (2 ≤ d < BASE).  Performs floor division. */
static void big_div_small(BigInt *a, unsigned int d) {
    if (d < 2u) return;
    uint64_t rem = 0;
    for (int i = (int)a->len - 1; i >= 0; i--) {
        uint64_t cur = a->d[i] + rem * BASE;
        a->d[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    big_trim(a);
}

/* Converts BigInt to decimal string.  The buffer must be large enough. */
static void big_to_str(const BigInt *a, char *buf, size_t bufsize) {
    if (a->len == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    size_t pos = 0;
    /* print most significant limb without leading zeros */
    int i = (int)a->len - 1;
    pos += snprintf(buf + pos, bufsize - pos, "%u", a->d[i]);
    i--;
    /* print remaining limbs with leading zeros */
    for (; i >= 0 && pos < bufsize - 1; i--) {
        pos += snprintf(buf + pos, bufsize - pos, "%09u", a->d[i]);
    }
    buf[pos] = '\0';
}

/* Global storage for the pre‑computed digits of e.  */
static char e_digits[PRECISION_DIGITS + 2];  /* +1 for '\0', +1 safety */
static bool e_precomputed = false;

/* Precompute e to PRECISION_DIGITS decimal digits. */
static void precompute_e_digits(void) {
    if (e_precomputed) return;
    /* Compute e * 10^PRECISION_DIGITS as an integer. */
    BigInt term, sum;
    /* term = 10^PRECISION_DIGITS */
    big_pow10(&term, PRECISION_DIGITS);
    term.len = term.len;  /* initial value is already set */
    /* sum = term */
    for (size_t i = 0; i < MAX_LIMBS; i++) sum.d[i] = 0u;
    sum.len = 0;
    big_add(&sum, &term);

    /* Iterate over the series 1/k! */
    unsigned int k = 1;
    while (true) {
        big_div_small(&term, k);   /* term = floor(10^N / k!) */
        if (term.len == 0) break;  /* term is zero => further terms are zero */
        big_add(&sum, &term);
        k++;
    }

    /* Convert sum to decimal string. */
    /* Expected length: PRECISION_DIGITS + 1 digits. */
    big_to_str(&sum, e_digits, sizeof(e_digits));
    e_precomputed = true;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* offset starts at 1 */
    precompute_e_digits();
    /* Ensure n does not exceed available digits (sanity check). */
    size_t ndx = (size_t)(n - 1);
    if (ndx >= strlen(e_digits)) return (seq_t)0;
    unsigned int d = (unsigned int)(e_digits[ndx] - '0');
    return (seq_t)(d * d);
}
