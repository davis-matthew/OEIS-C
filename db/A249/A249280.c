/*
 * A249280  Repeatedly apply 'Reverse and add' to n. a(n) gives the number of
 *          steps needed to reach a sum containing each digit from 0 to 9 at
 *          least once.
 * https://oeis.org/A249280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of reverse‑and‑add steps needed until the
 *            resulting number contains every decimal digit at least once
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t *d;   /* little‑endian digit array (LSB first) */
    size_t   len;
} Digits;

/* Convert a small integer (idx_t) to a Digits structure. */
static Digits digits_from_uint64(idx_t n) {
    if (n == 0) {
        uint8_t *arr = (uint8_t*)malloc(1);
        arr[0] = 0;
        Digits r; r.d = arr; r.len = 1; return r;
    }
    idx_t t = n;
    size_t len = 0;
    while (t > 0) { t /= 10; ++len; }
    uint8_t *arr = (uint8_t*)malloc(len);
    size_t i = 0;
    while (n > 0) {
        arr[i++] = n % 10;
        n /= 10;
    }
    Digits r; r.d = arr; r.len = len; return r;
}

/* Reverse the decimal representation (ignoring leading zeros of the
   reversed string).  Returns a new Digits structure that must be freed. */
static Digits reverse_digits(const Digits *orig) {
    size_t tz = 0;
    while (tz < orig->len && orig->d[tz] == 0) ++tz;
    size_t revlen = orig->len - tz;
    uint8_t *rev = (uint8_t*)malloc(revlen);
    size_t i;
    for (i = 0; i < revlen; ++i) {
        rev[i] = orig->d[orig->len - 1 - i];
    }
    Digits r; r.d = rev; r.len = revlen; return r;
}

/* Add two Digits numbers.  Returns a new Digits structure that must
   be freed by the caller. */
static Digits add_digits(const Digits *a, const Digits *b) {
    size_t len = a->len > b->len ? a->len : b->len;
    uint8_t *sum = (uint8_t*)malloc(len + 1);
    size_t i;
    uint8_t carry = 0;
    for (i = 0; i < len; ++i) {
        uint8_t da = (i < a->len) ? a->d[i] : 0;
        uint8_t db = (i < b->len) ? b->d[i] : 0;
        uint16_t s = da + db + carry;
        sum[i] = s % 10;
        carry  = s / 10;
    }
    if (carry) {
        sum[len] = carry;
        ++len;
    }
    Digits r; r.d = sum; r.len = len; return r;
}

/* Test whether the number represented by v contains all digits 0‑9. */
static bool all_digits_present(const Digits *v) {
    bool seen[10] = {false};
    size_t i;
    for (i = 0; i < v->len; ++i)
        seen[v->d[i]] = true;
    for (i = 0; i < 10; ++i)
        if (!seen[i]) return false;
    return true;
}

seq_t a(idx_t n) {
    seq_t steps = 0;

    Digits cur = digits_from_uint64(n);
    if (all_digits_present(&cur)) {
        free(cur.d);
        return steps;
    }

    while (1) {
        Digits rev = reverse_digits(&cur);
        Digits sum = add_digits(&cur, &rev);
        free(rev.d);
        free(cur.d);
        ++steps;

        if (all_digits_present(&sum)) {
            free(sum.d);
            break;
        }
        cur = sum;
    }

    return steps;
}
