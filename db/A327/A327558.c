/*
 * A327558  Decimal expansion of Sum_{k>=1} 1/F(k)! where F(k) = A000045(k).
 * https://oeis.org/A327558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 114, u32 n <= 114, u64 n <= 114,
 *            u128 n <= 114, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of S = Σ_{k≥1} 1/(Fib(k))! , where Fib(k) is the Fibonacci sequence.
   a(n) returns the n‑th decimal digit of S (a(1) = 2).  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Big integer type (base 1e9) ---------- */
typedef struct {
    size_t size;          /* number of used limbs (most significant limb at index size-1) */
    size_t capacity;      /* allocated limbs */
    uint32_t *limb;       /* least significant limb first */
} BigInt;

#define BASE 1000000000U

static void bi_grow(BigInt *b, size_t new_cap) {
    if (new_cap <= b->capacity) return;
    size_t cap = b->capacity ? b->capacity * 2 : 1;
    while (cap < new_cap) cap *= 2;
    uint32_t *ptr = (uint32_t *)realloc(b->limb, cap * sizeof(uint32_t));
    if (!ptr) abort();
    b->limb = ptr;
    b->capacity = cap;
}

static void bi_init(BigInt *b, size_t cap) {
    b->size = 0;
    b->capacity = cap;
    b->limb = cap ? (uint32_t *)malloc(cap * sizeof(uint32_t)) : NULL;
    if (cap && !b->limb) abort();
}

static void bi_free(BigInt *b) {
    free(b->limb);
    b->limb = NULL;
    b->size = b->capacity = 0;
}

static void bi_set_small(BigInt *b, uint32_t v) {
    bi_grow(b, 1);
    if (v == 0) {
        b->size = 0;
    } else {
        b->size = 1;
        b->limb[0] = v;
    }
}

static void bi_copy(BigInt *dst, const BigInt *src) {
    bi_grow(dst, src->size);
    dst->size = src->size;
    for (size_t i = 0; i < src->size; ++i) dst->limb[i] = src->limb[i];
}

static void bi_trim(BigInt *b) {
    while (b->size && b->limb[b->size-1] == 0) --b->size;
}

static int bi_compare(const BigInt *a, const BigInt *b) {
    if (a->size < b->size) return -1;
    if (a->size > b->size) return 1;
    for (size_t i = a->size; i-- > 0; ) {
        if (a->limb[i] < b->limb[i]) return -1;
        if (a->limb[i] > b->limb[i]) return 1;
    }
    return 0;
}

static void bi_add_inplace(BigInt *a, const BigInt *b) {
    size_t n = a->size > b->size ? a->size : b->size;
    bi_grow(a, n + 1);
    uint64_t carry = 0;
    for (size_t i = 0; i < n; ++i) {
        uint64_t sum = carry;
        if (i < a->size) sum += a->limb[i];
        if (i < b->size) sum += b->limb[i];
        a->limb[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) {
        a->limb[n] = (uint32_t)carry;
        a->size = n + 1;
    } else {
        a->size = n;
    }
}

static void bi_mul_small_inplace(BigInt *a, uint32_t m) {
    if (m == 0) { a->size = 0; return; }
    if (m == 1) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < a->size; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        a->limb[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        bi_grow(a, a->size + 1);
        a->limb[a->size] = (uint32_t)(carry % BASE);
        carry /= BASE;
        ++a->size;
    }
}

static void bi_mul_small(BigInt *res, const BigInt *a, uint32_t m) {
    bi_grow(res, a->size + 1);
    res->size = 0;
    if (m == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < a->size; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        res->limb[res->size++] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) {
        res->limb[res->size++] = (uint32_t)carry;
    }
    bi_trim(res);
}

static void bi_div_small_inplace(BigInt *a, uint32_t d) {
    if (d == 1 || a->size == 0) return;
    uint64_t rem = 0;
    for (size_t i = a->size; i-- > 0; ) {
        uint64_t cur = rem * BASE + a->limb[i];
        a->limb[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    bi_trim(a);
}

static void bi_sub_inplace(BigInt *a, const BigInt *b) {
    /* assumes a >= b */
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->size; ++i) {
        uint64_t bi = i < b->size ? b->limb[i] : 0;
        int64_t diff = (int64_t)a->limb[i] - (int64_t)bi - (int64_t)borrow;
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->limb[i] = (uint32_t)diff;
    }
    bi_trim(a);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    /* static state */
    static bool initialized = false;
    static BigInt factD;    /* (largest Fib)! == 6765! */
    static BigInt rem;      /* current remainder after extracting digits */
    static uint8_t *digits = NULL;
    static idx_t digits_len = 0;
    static size_t digits_cap = 0;
    if (!initialized) {
        /* 1. compute 6765! */
        bi_init(&factD, 0);
        bi_set_small(&factD, 1);
        for (unsigned i = 2; i <= 6765; ++i) bi_mul_small_inplace(&factD, i);

        /* 2. sum terms */
        BigInt numerator, term, temp;
        bi_init(&numerator, 0);
        bi_set_small(&numerator, 0);
        bi_copy(&term, &factD);   /* term = factD initially (for f=1) */
        const unsigned fibs[] = {1,1,2,3,5,8,13,21,34,55,89,144,233,377,610,
                                 987,1597,2584,4181,6765};
        const size_t fib_cnt = sizeof(fibs)/sizeof(fibs[0]);

        unsigned prev_f = 0;
        for (size_t i = 0; i < fib_cnt; ++i) {
            unsigned f = fibs[i];
            if (f > prev_f) {
                for (unsigned m = prev_f + 1; m <= f; ++m)
                    bi_div_small_inplace(&term, m);
            }
            /* term now equals factD / f!  */
            bi_add_inplace(&numerator, &term);
            prev_f = f;
        }

        /* 3. compute integer part and remainder */
        int int_digit = 0;
        bi_init(&temp, 0);
        for (int q = 1; q <= 9; ++q) {
            bi_mul_small(&temp, &factD, (uint32_t)q);
            if (bi_compare(&temp, &numerator) > 0) break;
            int_digit = q;
        }
        bi_copy(&rem, &numerator);
        if (int_digit > 0) {
            bi_mul_small(&temp, &factD, (uint32_t)int_digit);
            bi_sub_inplace(&rem, &temp);
        }

        /* 4. initialize digit array */
        digits_cap = 20000;                 /* enough for expected n */
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        if (!digits) abort();
        digits[0] = (uint8_t)int_digit;
        digits_len = 1;

        /* free temporaries */
        bi_free(&numerator);
        bi_free(&term);
        bi_free(&temp);
        /* keep factD and rem for later use */
        initialized = true;
    }

    /* compute more digits if necessary */
    static BigInt denom;      /* equals factD but we keep for clarity */
    static bool denom_init = false;
    if (!denom_init) {
        bi_copy(&denom, &factD);
        denom_init = true;
    }
    static BigInt temp;       /* reused for temporary computations */
    while (n > digits_len) {
        /* remainder *= 10 */
        bi_mul_small_inplace(&rem, 10);

        /* find next digit (0..9) */
        int qdigit = 0;
        for (int q = 1; q <= 9; ++q) {
            bi_mul_small(&temp, &denom, (uint32_t)q);
            if (bi_compare(&temp, &rem) > 0) break;
            qdigit = q;
        }
        /* rem -= denom * qdigit */
        if (qdigit > 0) {
            bi_mul_small(&temp, &denom, (uint32_t)qdigit);
            bi_sub_inplace(&rem, &temp);
        }

        /* store digit */
        if (digits_len >= digits_cap) {
            digits_cap *= 2;
            uint8_t *tmp = (uint8_t *)realloc(digits, digits_cap);
            if (!tmp) abort();
            digits = tmp;
        }
        digits[digits_len++] = (uint8_t)qdigit;
    }
    return (seq_t)digits[n-1];
}
