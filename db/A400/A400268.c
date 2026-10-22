/*
 * A400268  Numbers that can be obtained by concatenating the decimal
 *          representations of x, y, and x^y for x >= 1 and y >= 0.
 * https://oeis.org/A400268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   OEIS A400268 – the concatenation of x, y and x^y
 * Range:     u8 none, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the n‑th term on demand, using a
 * min‑heap of the form x, y, pow(x,y).  It works for any
 * integer type seq_t, including arbitrary‑precision types.
 * No global memoisation is required. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"

/* ----------------------------------------------------------------- */
/* Helper: overflow‑safe multiplication and addition for seq_t      */
/* ----------------------------------------------------------------- */

static bool mul_overflow(seq_t a, seq_t b, seq_t *out)
{
    if (b == 0) { *out = 0; return false; }
    seq_t res = a * b;
    if (b != 0 && res / b != a) return true;
    *out = res;
    return false;
}

static bool add_overflow(seq_t a, seq_t b, seq_t *out)
{
    seq_t res = a + b;
    if (res < a) return true;
    *out = res;
    return false;
}

/* ----------------------------------------------------------------- */
/* Pre‑computed powers of ten – up to the first overflow            */
/* ----------------------------------------------------------------- */

#define MAX_POW10 30
static seq_t pow10_arr[MAX_POW10 + 1];
static bool pow10_built = false;

static void build_pow10_array(void)
{
    if (pow10_built) return;
    pow10_arr[0] = 1;
    seq_t prev = 1;
    for (int i = 1; i <= MAX_POW10; ++i) {
        seq_t nxt;
        if (mul_overflow(prev, (seq_t)10, &nxt)) {
            pow10_arr[i] = 0;          /* overflow – mark as unusable */
            break;
        }
        pow10_arr[i] = nxt;
        prev = nxt;
    }
    pow10_built = true;
}

/* ----------------------------------------------------------------- */
/* Heap node                                                    */
/* ----------------------------------------------------------------- */

typedef struct {
    seq_t val;      /* concatenated number */
    idx_t x;        /* base */
    idx_t y;        /* exponent */
    seq_t pow_xy;   /* current value of x^y */
} Node;

/* ----------------------------------------------------------------- */
/* Tiny binary min‑heap for Node objects                            */
/* ----------------------------------------------------------------- */

static Node *heap = NULL;
static size_t heap_sz = 0, heap_cap = 0;

static void heap_init(size_t cap)
{
    heap_cap = cap;
    heap_sz = 0;
    heap = (Node*)malloc(cap * sizeof(Node));
}

static void heap_free(void)
{
    free(heap);
    heap = NULL;
    heap_sz = heap_cap = 0;
}

static void heap_push(Node n)
{
    if (heap_sz == heap_cap) {
        heap_cap = (heap_cap > 0) ? heap_cap * 2 : 64;
        heap = (Node*)realloc(heap, heap_cap * sizeof(Node));
    }
    size_t i = heap_sz++;
    while (i > 0) {
        size_t p = (i - 1) / 2;
        if (heap[p].val <= n.val) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = n;
}

static Node heap_pop(void)
{
    Node top = heap[0];
    heap_sz--;
    if (heap_sz > 0) {
        Node last = heap[heap_sz];
        size_t i = 0;
        while (1) {
            size_t l = i * 2 + 1;
            if (l >= heap_sz) break;
            size_t r = l + 1;
            size_t child = (r < heap_sz && heap[r].val < heap[l].val) ? r : l;
            if (heap[child].val >= last.val) break;
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = last;
    }
    return top;
}

/* ----------------------------------------------------------------- */
/* Main function: returns the n‑th term of A400268                    */
/* ----------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* out of range – but never requested */

    build_pow10_array();

    /* Initial bound for x – enough to produce >10 000 terms. */
    const idx_t X_MAX = 5000;

    heap_init(X_MAX);

    /* Push the initial y = 0 term for every x (x^0 = 1). */
    const seq_t pow10_2 = pow10_arr[2];   /* 10^(1+1) = 100 */
    const seq_t pow10_1 = pow10_arr[1];   /* 10^1 = 10   */
    for (idx_t x = 1; x <= X_MAX; ++x) {
        /* Term for y = 0:  x*100 + 1 */
        seq_t t1, val;
        if (mul_overflow(x, pow10_2, &t1)) continue;
        if (add_overflow(t1, (seq_t)1, &val)) continue;
        Node node = { val, x, 0, 1 }; /* pow_xy = 1 for y = 0 */
        heap_push(node);
    }

    idx_t count = 0;
    seq_t last_val = 0;
    bool first = true;

    while (count < n) {
        Node cur = heap_pop();

        /* Skip duplicate values */
        if (first || cur.val != last_val) {
            last_val = cur.val;
            count++;
            if (count == n) {
                heap_free();
                return cur.val;
            }
            first = false;
        }

        /* Generate the next term for the same base x:
         * y' = cur.y + 1 ;  pow_xy' = cur.pow_xy * x
         */
        idx_t x = cur.x;
        idx_t y_next = cur.y + 1;

        seq_t pow_xy_next;
        if (mul_overflow(cur.pow_xy, (seq_t)x, &pow_xy_next)) {
            /* Overflow – stop generating more y for this x */
            continue;
        }

        int q = digit_len((seq_t)y_next);
        int r = digit_len(pow_xy_next);

        if (q + r > MAX_POW10 || r > MAX_POW10 ||
            pow10_arr[q + r] == 0 || pow10_arr[r] == 0) {
            continue;   /* term would overflow seq_t */
        }

        seq_t t1, t2, sum, val;
        if (mul_overflow(x, pow10_arr[q + r], &t1)) continue;
        if (mul_overflow((seq_t)y_next, pow10_arr[r], &t2)) continue;
        if (add_overflow(t1, t2, &sum)) continue;
        if (add_overflow(sum, pow_xy_next, &val)) continue;

        Node next = { val, x, y_next, pow_xy_next };
        heap_push(next);
    }

    /* Should never reach here */
    heap_free();
    return 0;
}
