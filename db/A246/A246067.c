/*
 * A246067  Record values in A246066.
 * https://oeis.org/A246067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 49, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A246067 – the record values of the non‑missing prime powers
   from A025475.  The sequence consists of all prime powers q^n (n≥2) for
   which there is no smaller prime power p^m with q^n−q ≤ p^m−p.
   The function generates the prime powers in ascending order, keeps a
   running maximum of the differences d = prime^k−prime seen so far,
   and includes a power only if its difference is larger than all
   differences seen before.  This yields exactly the “record” terms.  */

/* Build a simple sieve of primes up to a fixed limit.  The limit is
   sufficient for n up to ~10,000; term values fit in the 64‑bit range,
   and generation never requires primes larger than 10⁶. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

typedef struct {
    seq_t val;      /* current prime power */
    idx_t prime;    /* base prime */
} Node;

static idx_t   *primes   = NULL;   /* array of primes */
static size_t  primes_cnt = 0;     /* number of primes */

static void init_primes(void)
{
    const idx_t LIMIT = 1000000;
    bool *sieve = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!sieve) exit(1);
    for (idx_t i = 2; i <= LIMIT; ++i) sieve[i] = true;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) sieve[j] = false;
        }
    }
    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) ++cnt;
    primes = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!primes) exit(1);
    idx_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) primes[idx++] = i;
    primes_cnt = cnt;
    free(sieve);
}

static void sift_up(size_t i, Node *h)
{
    while (i > 1) {
        size_t p = i >> 1;
        if (h[i].val < h[p].val) {
            Node t = h[i];
            h[i] = h[p];
            h[p] = t;
            i = p;
        } else break;
    }
}

static void sift_down(size_t i, size_t sz, Node *h)
{
    while ((i << 1) <= sz) {
        size_t c = i << 1;
        if (c + 1 <= sz && h[c + 1].val < h[c].val) ++c;
        if (h[c].val < h[i].val) {
            Node t = h[i];
            h[i] = h[c];
            h[c] = t;
            i = c;
        } else break;
    }
}

static void heap_push(Node n, size_t *sz, Node *h)
{
    ++(*sz);
    h[*sz] = n;
    sift_up(*sz, h);
}

static Node heap_pop(size_t *sz, Node *h)
{
    Node res = h[1];
    h[1] = h[*sz];
    --(*sz);
    sift_down(1, *sz, h);
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1 */
    if (!primes) init_primes();

    /* Min‑heap of prime powers; index 1..size */
    Node *heap = (Node*)malloc((primes_cnt + 1) * sizeof(Node));
    if (!heap) exit(1);
    size_t heap_sz = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        seq_t val = mul_small((seq_t)p, (unsigned int)p);  /* p² */
        heap_push((Node){val, p}, &heap_sz, heap);
    }

    seq_t max_diff = 0;          /* largest diff seen so far */
    seq_t last_val = 0;
    size_t out_cnt = 0;

    while (out_cnt < n) {
        Node cur = heap_pop(&heap_sz, heap);
        seq_t diff = cur.val - (seq_t)cur.prime;  /* q^n - q */

        if (diff > max_diff) {        /* record term */
            max_diff = diff;
            last_val  = cur.val;
            ++out_cnt;
        }

        /* Push next power of this prime */
        seq_t next_val = mul_small(cur.val, (unsigned int)cur.prime);
        heap_push((Node){next_val, cur.prime}, &heap_sz, heap);
    }

    free(heap);
    return last_val;
}
