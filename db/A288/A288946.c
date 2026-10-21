/*
 * A288946  Starting with a(1) = 1, a(n) = smallest nonnegative integer not
 *          yet in the sequence such that the first digit of a(n) is plus or
 *          minus one the last digit of a(n-1) plus or minus 1, with
 *          preference given +1 when possible. The digit 0 is not allowed.
 * https://oeis.org/A288946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.  For n>1, a(n) is the smallest unused
   non‑negative integer with no zero digit such that its first digit
   is the previous number's last digit + 1 (if <=9), otherwise 8
   (i.e. a forced decrease when the last digit is 9).  Preference
   is given to the +1 option when possible.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"

/* Return the first (most significant) decimal digit of x */
static inline int first_digit(seq_t x)
{
    while (x >= 10) x /= 10;
    return (int)x;
}

/* Return the length in decimal digits of x (x>0) */
static inline int digit_len(seq_t x)
{
    int len = 0;
    do { ++len; x /= 10; } while (x);
    return len;
}

/* Return true if x contains a zero digit */
static inline bool has_zero(seq_t x)
{
    while (x) {
        if ((x % 10) == 0) return true;
        x /= 10;
    }
    return false;
}

/* Bitset of used numbers.  The index is assumed to fit in idx_t. */
static unsigned char *used = NULL;
static size_t used_cap = 0;

static void ensure_used_idx(idx_t v)
{
    if ((idx_t)v >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 1024;
        while ((idx_t)new_cap <= v) new_cap <<= 1;
        used = (unsigned char *)realloc(used, new_cap);
        if (!used) abort();
        for (size_t i = used_cap; i < new_cap; ++i) used[i] = 0;
        used_cap = new_cap;
    }
}

static void mark_used(seq_t v)
{
    idx_t iv = (idx_t)v;
    ensure_used_idx(iv);
    used[iv] = 1;
}

static bool is_used(seq_t v)
{
    idx_t iv = (idx_t)v;
    return iv < used_cap && used[iv];
}

/* Return the first unused number >= start whose first digit equals d
   and that contains no zero digit.  The search advances start until
   a suitable number is found. */
static seq_t find_next_with_leading_digit(int d, seq_t start)
{
    seq_t v = start;
    for (;;) {
        int fd = first_digit(v);
        int len = digit_len(v);

        if (fd != d) {
            if (fd < d)
                v = (seq_t)d * pow10((idx_t)(len - 1));
            else
                v = (seq_t)d * pow10((idx_t)len);
        }

        if (has_zero(v) || is_used(v)) {
            ++v;
            continue;
        }
        return v;
    }
}

/* Return the next unused number with leading digit d and update ptr */
static seq_t get_next_with_digit(int d, seq_t *ptr)
{
    seq_t start = *ptr;
    seq_t val = find_next_with_leading_digit(d, start);
    *ptr = val + 1;
    return val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* dynamic array of sequence terms */
    static idx_t seq_len = 0;      /* number of terms currently stored */
    static seq_t next_ptr[10];     /* next candidate for each leading digit 1..9 */
    static bool initialized = false;

    if (!initialized) {
        /* initial allocation */
        size_t init_cap = 128;
        seq = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!seq) abort();
        for (int d = 1; d <= 9; ++d) next_ptr[d] = d;
        seq_len = 0;
        initialized = true;
    }

    /* Expand sequence up to requested index */
    while (seq_len < n) {
        seq_t val;
        if (seq_len == 0) {
            val = 1;
        } else {
            int last_digit = (int)(seq[seq_len - 1] % 10);
            int up = (last_digit < 9) ? last_digit + 1 : -1;
            int low = (last_digit > 1) ? last_digit - 1 : -1;
            if (up != -1)
                val = get_next_with_digit(up, &next_ptr[up]);
            else
                val = get_next_with_digit(low, &next_ptr[low]);
        }

        /* Grow the array if necessary */
        if (seq_len == 0 && !seq) {
            seq = (seq_t *)malloc(sizeof(seq_t));
            if (!seq) abort();
        }
        /* Since we never shrink, we just check for potential overflow:
           the following test assumes seq_len fits in size_t. */
        if ((size_t)seq_len + 1 > (size_t)(-1) / sizeof(seq_t)) {
            /* array would overflow – simply abort */
            abort();
        }

        seq[seq_len] = val;
        mark_used(val);
        ++seq_len;
    }

    return seq[n - 1];
}
