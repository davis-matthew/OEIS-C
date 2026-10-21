/*
 * A278833  Indices of records of A004090(n) - n, where A004090 is the sum of
 *          digits of the Fibonacci numbers A000045.
 * https://oeis.org/A278833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th index of a strict record of *
 *            sum_of_digits(Fibonacci(n)) - n * where a record occurs when the
 *            value is greater than all * previous values
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

/* static state */
static idx_t *record_idx = NULL;   /* array of record indices */
static size_t record_len = 0;
static size_t record_cap = 0;

static int64_t max_diff = LLONG_MIN; /* largest diff seen so far */
static idx_t last_n = UINT64_MAX;     /* index processed most recently */

/* simple decimal big integer */
typedef struct {
    uint8_t *d;     /* least‑significant digit first, base 10 */
    size_t len;     /* number of used digits, len >= 1 except for zero */
} Big;

static Big fprev; /* Fib(n-1) */
static Big fcurr; /* Fib(n)   */

/* append a new record index to the static array */
static void push_record(idx_t v)
{
    if (record_len == record_cap) {
        size_t new_cap = record_cap ? record_cap * 2 : 16;
        idx_t *p = (idx_t *)realloc(record_idx, new_cap * sizeof(idx_t));
        if (!p) exit(1);            /* memory exhausted */
        record_idx = p;
        record_cap = new_cap;
    }
    record_idx[record_len++] = v;
}

/* initialise the static state the first time the function is called */
static void init_sequence(void)
{
    /* Fib(0) */
    fprev.d = (uint8_t *)malloc(1);
    if (!fprev.d) exit(1);
    fprev.d[0] = 0;
    fprev.len = 1;

    /* Fib(1) */
    fcurr.d = (uint8_t *)malloc(1);
    if (!fcurr.d) exit(1);
    fcurr.d[0] = 1;
    fcurr.len = 1;

    /* record for n = 0 */
    push_record(0);
    max_diff = 0;      /* sum_of_digits(0) - 0 = 0 */

    /* diff for n = 1 (Fib(1) = 1) */
    int sum1 = 1;                      /* digit sum of Fib(1) */
    int64_t diff1 = (int64_t)sum1 - 1; /* 0, not a new record */
    last_n = 1;
}

seq_t a(idx_t n)
{
    if (!record_idx) init_sequence();

    while (record_len < n) {
        idx_t next_n = last_n + 1;

        /* compute Fib(next_n) = fprev + fcurr in decimal */
        size_t maxlen = fprev.len > fcurr.len ? fprev.len : fcurr.len;
        size_t needed = maxlen + 1;
        uint8_t *buf = (uint8_t *)malloc(needed);
        if (!buf) exit(1);

        uint8_t carry = 0;
        size_t i;
        int digit_sum = 0;
        for (i = 0; i < maxlen; ++i) {
            uint8_t da = (i < fprev.len) ? fprev.d[i] : 0;
            uint8_t db = (i < fcurr.len) ? fcurr.d[i] : 0;
            uint8_t s = da + db + carry;
            buf[i] = s % 10;
            carry = s / 10;
            digit_sum += buf[i];
        }
        if (carry) {
            buf[i++] = carry;
            digit_sum += carry;
        }
        size_t fnext_len = i;

        int64_t diff = (int64_t)digit_sum - (int64_t)next_n;
        if (diff > max_diff) {
            push_record(next_n);
            max_diff = diff;
        }

        /* shift for next iteration */
        free(fprev.d);
        fprev.d = fcurr.d; fprev.len = fcurr.len;
        fcurr.d = buf;     fcurr.len = fnext_len;

        last_n = next_n;
    }

    return (seq_t)record_idx[n - 1];
}
