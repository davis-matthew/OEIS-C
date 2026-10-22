/*
 * A323419  The fourth greedy 3-free sequence on nonnegative integers.
 * https://oeis.org/A323419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th smallest integer whose base‑3 representation ends in
 *            10, 11, 20, or 21 and contains exactly one digit '2' before that
 *            suffix (the suffix itself may contain a second '2', which is
 *            part of the final two digits). This algorithm enumerates all
 *            such numbers by treating each choice of the p...
 * Range:     u8 n <= 4, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

typedef struct {
    seq_t val;   /* current value of the progression */
    seq_t step;  /* 3^(k+3), constant for the progression */
    seq_t base;  /* 2*3^(k+2) + suffix, constant part of the progression */
    idx_t  m;    /* current binary counter value (lifts into T = {numbers with only digits 0 or 1 in base 3}) */
    seq_t t_val; /* current value of T(m) = interpret m in base 3 */
} Stream;

/* Min–heap routine */
static void heapify_down(Stream *h, size_t size, size_t i)
{
    while (true) {
        size_t l = 2 * i + 1;
        size_t r = 2 * i + 2;
        size_t smallest = i;

        if (l < size && h[l].val < h[smallest].val) smallest = l;
        if (r < size && h[r].val < h[smallest].val) smallest = r;

        if (smallest == i) break;

        Stream tmp = h[i];
        h[i] = h[smallest];
        h[smallest] = tmp;
        i = smallest;
    }
}

/* Number of allowed suffixes */
static const seq_t suffix_vals[4] = {3, 4, 6, 7};

/* Maximum k (position of the unique '2') that is required for the first
   10 000 terms while keeping 64‑bit arithmetic safe.
   The necessary bound is 24 (k+3 ≤ 27 ⇒ step ≤ 3^27 < 7.7·10^12). */
#define KMAX 24

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for non‑positive input */

    /* Pre‑compute powers of 3 up to exponent 27 (KMAX+3) and up to bit
       indices needed for updating T(m).  3^27 fits in 64‑bit (≈7.6e12). */
    const size_t POW_CNT = (KMAX + 3 + 15);   /* 27 + 15 = 42, safe */
    seq_t pow3[POW_CNT + 1];
    pow3[0] = (seq_t)1;
    for (size_t i = 1; i <= POW_CNT; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* Number of streams: 4 suffixes for each k (0..KMAX) */
    const size_t STREAM_CNT = 4 * (KMAX + 1);
    Stream heap[STREAM_CNT];
    size_t heap_sz = 0;

    /* Initialise each stream with m = 0 and compute initial value */
    for (size_t k = 0; k <= KMAX; ++k) {
        seq_t pow_k2  = pow3[k + 2];
        seq_t step    = pow3[k + 3];
        for (int s = 0; s < 4; ++s) {
            seq_t base = (seq_t)2 * pow_k2 + suffix_vals[s];
            Stream *st = &heap[heap_sz++];
            st->step   = step;
            st->base   = base;
            st->m      = 0;
            st->t_val  = 0;
            st->val    = base;            /* base + step * 0 */
        }
    }

    /* Build initial min‑heap */
    for (size_t i = heap_sz / 2; i > 0; --i)
        heapify_down(heap, heap_sz, i - 1);

    seq_t result = 0;

    /* Extract the n‑th smallest value */
    for (idx_t cnt = 1; cnt <= n; ++cnt) {
        result = heap[0].val;

        if (cnt == n)
            return result;                /* final answer */

        /* Advance the stream that produced the smallest value */
        Stream *st = &heap[0];
        idx_t old_m = st->m;
        st->m = old_m + 1;

        /* Find position of first zero bit in old_m (least significant zero) */
        idx_t pos = 0;
        idx_t tmp = old_m;
        while ((tmp & 1) != 0) {
            tmp >>= 1;
            pos++;
        }

        /* delta = (3^pos + 1) / 2, integer because 3^pos is odd */
        seq_t delta = (pow3[pos] + (seq_t)1) / (seq_t)2;

        st->t_val += delta;
        st->val += st->step * delta;

        heapify_down(heap, heap_sz, 0);
    }

    return result; /* ne‑reach */
}
