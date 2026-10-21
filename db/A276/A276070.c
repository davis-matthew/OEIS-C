/*
 * A276070  Each digit of the absolute difference |a(n+1)-a(n)| is also a
 *          digit of a(n): lexicographic first such sequence with no repeated
 *          term.
 * https://oeis.org/A276070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; for n>1, a(n) is the smallest unused positive integer
 *            such that every decimal digit of |a(n)-a(n-1)| appears as a
 *            digit of a(n-1)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence iteratively, keeping a memoized array of terms and a bit‑set of used integers. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *seq = NULL;      /* memoized sequence */
static size_t seq_cap = 0;
static size_t seq_len = 0;     /* number of elements stored */

static uint8_t *used = NULL;   /* bit‑set of used integers (index = integer) */
static size_t used_cap = 0;

/* Ensure the sequence array has room for at least 'needed' elements. */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 1024;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq = tmp;
    seq_cap = newcap;
}

/* Ensure the used array has room for at least 'needed' indices.  */
static void ensure_used_cap(size_t needed)
{
    if (needed <= used_cap) return;
    size_t newcap = used_cap ? used_cap * 2 : 1024;
    while (newcap < needed) newcap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, newcap * sizeof(uint8_t));
    /* initialise new part to 0 */
    for (size_t i = used_cap; i < newcap; ++i) tmp[i] = 0;
    used = tmp;
    used_cap = newcap;
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0; offset is 1 */
    size_t target = (size_t)n;

    /* fast return if already computed */
    if (target <= seq_len) return seq[target - 1];

    /* initialise the first term if necessary */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        ensure_used_cap(2);
        used[1] = 1;
    }

    /* generate terms up to 'target' */
    while (seq_len < target) {
        seq_t prev = seq[seq_len - 1];

        /* build digit mask of 'prev' */
        bool prev_digits[10] = { false };
        seq_t tmp = prev;
        if (tmp == 0) prev_digits[0] = true;
        while (tmp > 0) {
            prev_digits[(int)(tmp % 10)] = true;
            tmp /= 10;
        }

        idx_t m = 1;                     /* candidate integer */
        for (;;) {
            if ((size_t)m >= used_cap) ensure_used_cap((size_t)m + 1);
            if (!used[(size_t)m]) {
                seq_t m_t = (seq_t)m;
                seq_t diff = (m_t > prev) ? m_t - prev : prev - m_t;
                seq_t d = diff;
                bool ok = true;

                if (d == 0) {            /* should not happen, but guard */
                    if (!prev_digits[0]) ok = false;
                } else {
                    while (d > 0) {
                        int digit = (int)(d % 10);
                        if (!prev_digits[digit]) { ok = false; break; }
                        d /= 10;
                    }
                }

                if (ok) {
                    ensure_seq_cap(seq_len + 1);
                    seq[seq_len] = m_t;
                    used[(size_t)m] = 1;
                    ++seq_len;
                    break;               /* candidate found */
                }
            }
            ++m;
        }
    }

    return seq[target - 1];
}
