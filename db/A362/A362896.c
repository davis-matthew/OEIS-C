/*
 * A362896  a(0)=2. For n>0, let d = n-th digit in the sequence thus far. a(n)
 *          = a(n-1) + d if d is even. Otherwise, a(n) = a(n-1) - d.
 * https://oeis.org/A362896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, base
 * Formula:   a(0)=2. For n>0, let d be the n‑th digit (ignoring minus signs)
 *            in the decimal concatenation of a(0),a(1),…,a(n‑1). Then
 *            a(n)=a(n‑1)+d if d is even, otherwise a(n)=a(n‑1)-d
 * Range:     i8 n <= 254, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps two dynamic arrays:
   * vals  – the computed sequence values,
   * digits – the stream of digits built up so far.
   The arrays grow only as needed and are reused by subsequent calls
   regardless of the order in which a(n) is requested. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --- dynamic array for sequence values ------------------------------------- */
static seq_t *vals      = NULL;
static size_t vals_len  = 0;   /* number of values currently stored        */
static size_t vals_cap  = 0;   /* allocated capacity                        */

static void ensure_vals_capacity(size_t needed)
{
    if (needed <= vals_cap) return;
    size_t newcap = vals_cap ? vals_cap * 2 : 8;
    if (newcap < needed) newcap = needed;
    seq_t *newptr = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (newptr == NULL) abort();          /* allocation failure */
    if (vals) {
        for (size_t i = 0; i < vals_len; ++i) newptr[i] = vals[i];
        free(vals);
    }
    vals = newptr;
    vals_cap = newcap;
}

/* --- dynamic array for the digit stream ----------------------------------- */
static uint8_t *digits  = NULL;
static size_t   diglen  = 0;   /* number of digits stored so far           */
static size_t   digcap  = 0;   /* allocated capacity                        */

static void ensure_digits_capacity(size_t needed)
{
    if (needed <= digcap) return;
    size_t newcap = digcap ? digcap * 2 : 8;
    if (newcap < needed) newcap = needed;
    uint8_t *newptr = (uint8_t *)realloc(digits, newcap);
    if (newptr == NULL) abort();
    digits = newptr;
    digcap = newcap;
}

/* --- append digits of a value to the global digit stream ------------------- */
static void append_value_digits(seq_t v)
{
    seq_t aa = v >= 0 ? v : -v;          /* absolute value, sign ignored */

    /* Collect digits in reverse order into a small temporary buffer. */
    uint8_t tmpbuf[64];
    int len = 0;
    if (aa == 0) {
        tmpbuf[0] = 0;
        len = 1;
    } else {
        while (aa != 0) {
            tmpbuf[len++] = (uint8_t)(aa % 10);
            aa /= 10;
        }
    }

    /* Append them to the global stream in normal order. */
    ensure_digits_capacity(diglen + (size_t)len);
    for (int i = len - 1; i >= 0; --i)
        digits[diglen++] = tmpbuf[i];
}

/* --------------------------------------------------------------------------- */
/* required public function */

seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (vals_len == 0) {
        ensure_vals_capacity(1);
        vals[0] = (seq_t)2;
        vals_len = 1;
        append_value_digits(vals[0]);                     /* digit '2' */
    }

    /* Compute any missing values up to index n */
    for (size_t k = vals_len; k <= (size_t)n; ++k) {
        if (k == 0) continue;        /* a(0) already present */

        uint8_t d   = digits[k - 1];  /* n‑th digit (1‑based) */
        seq_t  prev = vals[k - 1];
        seq_t  curr = ((d & 1) == 0) ? (prev + d) : (prev - d);

        ensure_vals_capacity(k + 1);
        vals[k] = curr;
        append_value_digits(curr);
        ++vals_len;
    }

    return vals[(size_t)n];
}
