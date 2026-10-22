/*
 * A378329  Decimal expansion of the base 5 Champernowne constant.
 * https://oeis.org/A378329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the (n+1)-th decimal digit of the base‑5 Champernowne
 *            constant. Algorithm: 1. Determine the number K of base‑5 digits
 *            that must be generated so that the first n+1 decimal digits are
 *            stable: K = ceil((n+2) * log_5(10)) 2. Generate the first K
 *            digits of the Champernowne string in base 5, bui...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* ---------- small integer big integer (base 1e9) ---------- */
#define BIGBASE 1000000000u

typedef struct {
    uint32_t *data; /* little‑endian words, each < BIGBASE */
    size_t size;    /* number of used words */
} BigInt;

static void bigint_init(BigInt *b) {
    b->data = NULL;
    b->size = 0;
}

static void bigint_free(BigInt *b) {
    if (b->data) free(b->data);
    b->data = NULL;
    b->size = 0;
}

static void bigint_ensure(BigInt *b, size_t needed) {
    if (b->size < needed) {
        size_t new_cap = b->size ? b->size * 2 : 8;
        while (new_cap < needed) new_cap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(b->data, new_cap * sizeof(uint32_t));
        if (!tmp) abort(); /* allocation failure */
        b->data = tmp;
    }
}

static void bigint_mul_small(BigInt *b, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < b->size; ++i) {
        uint64_t prod = (uint64_t)b->data[i] * m + carry;
        b->data[i] = (uint32_t)(prod % BIGBASE);
        carry = prod / BIGBASE;
    }
    while (carry) {
        bigint_ensure(b, b->size + 1);
        b->data[b->size++] = (uint32_t)(carry % BIGBASE);
        carry /= BIGBASE;
    }
}

static void bigint_add_small(BigInt *b, uint32_t m) {
    uint64_t carry = m;
    size_t i = 0;
    for (; i < b->size; ++i) {
        uint64_t sum = (uint64_t)b->data[i] + carry;
        b->data[i] = (uint32_t)(sum % BIGBASE);
        carry = sum / BIGBASE;
        if (!carry) break;
    }
    if (carry) {
        bigint_ensure(b, b->size + 1);
        b->data[b->size++] = (uint32_t)carry;
    }
}

/* Convert BigInt to decimal string (null terminated).  Caller must free buf. */
static void bigint_to_decimal(const BigInt *b, char **buf, size_t *len) {
    if (b->size == 0) {
        *buf = (char *)malloc(2);
        (*buf)[0] = '0';
        (*buf)[1] = '\0';
        *len = 1;
        return;
    }

    /* Find most significant non‑zero word */
    size_t msw = b->size - 1;
    while (msw > 0 && b->data[msw] == 0) msw--;

    /* Estimate maximum length: 9 digits per word + 1 for NUL */
    size_t maxlen = msw * 9 + 10;
    char *tmp = (char *)malloc(maxlen);
    if (!tmp) abort();

    size_t pos = 0;
    /* Print most significant word without leading zeros */
    int n = snprintf(tmp + pos, maxlen - pos, "%u", b->data[msw]);
    pos += n;
    /* Remaining words with leading zeros to 9 digits */
    for (size_t i = msw; i-- > 0;) {
        snprintf(tmp + pos, maxlen - pos, "%09u", b->data[i]);
        pos += 9;
    }
    tmp[pos] = '\0';
    *buf = tmp;
    *len = pos;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* 1. Determine K = ceil((n+2)*log5(10)) */
    static const double LOG5_10 = 1.430677093745337;   /* log base 5 of 10 */
    double kf = (n + 2) * LOG5_10;
    idx_t K = (idx_t)kf;
    if ((double)K < kf - 1e-12) ++K;  /* ceil */

    /* 2. Generate first K base‑5 digits and build the integer M */
    BigInt M;
    bigint_init(&M);

    idx_t remaining = K;
    uint64_t cur = 1;         /* natural number to encode */
    uint8_t buffer[64];       /* temporary holder for a number's digits */

    while (remaining > 0) {
        /* Convert cur to base‑5 digits (most significant first) */
        uint8_t dgt[64];
        int dlen = 0;
        uint64_t t = cur;
        do {
            dgt[dlen++] = t % 5;
            t /= 5;
        } while (t > 0);
        /* Append digits */
        for (int i = dlen - 1; i >= 0; --i) {
            uint8_t d = dgt[i];
            bigint_mul_small(&M, 5);
            bigint_add_small(&M, d);
            if (--remaining == 0) break;
        }
        ++cur;
    }

    /* 3. Compute N = M * 2^K */
    for (idx_t i = 0; i < K; ++i) {
        bigint_mul_small(&M, 2);
    }

    /* 4. Convert N to decimal string */
    char *dec_str = NULL;
    size_t dec_len = 0;
    bigint_to_decimal(&M, &dec_str, &dec_len);

    /* 5. Determine digit */
    seq_t res = 0;
    idx_t pad = K - dec_len;      /* leading zeros count */
    if (n >= pad) {
        res = (seq_t)(dec_str[n - pad] - '0');
    } else {
        res = 0;
    }

    /* Clean up */
    free(dec_str);
    bigint_free(&M);

    return res;
}
