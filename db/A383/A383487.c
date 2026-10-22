/*
 * A383487  Prime powers p^r such that p == 3 (mod 4) and r is even.
 * https://oeis.org/A383487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A383487: prime powers p^r with p ≡ 3 (mod 4) and 
   even exponent r, sorted ascending.  a(1) = 9, a(2) = 49, … 
   The function returns the n‑th term for any n ≥ 1.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* Forward declaration of the required types.  The compiler provides
   appropriate typedefs for seq_t and idx_t. */
extern seq_t a(idx_t n);

/*======================================================================
 *  Helper for generating all primes ≡ 3 mod 4 up to a fixed limit.
 *====================================================================*/
static uint32_t *primes3 = NULL;
static size_t    prime_count = 0;

/* Once per program execution: create the list of primes ≡ 3 modulo 4. */
static void generate_primes3(void)
{
    if (primes3 != NULL) return;   /* already generated */

    /* Upper bound sufficient for n ≤ 20000; more than enough for 10000. */
    const size_t LIMIT = 400000;          /* 4·10^5 */
    uint8_t *sieve = (uint8_t *)malloc(LIMIT + 1);
    if (!sieve) return;                  /* out of memory */

    for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;

    /* Count how many primes we will keep */
    prime_count = 0;
    for (size_t i = 3; i <= LIMIT; i += 2)
        if (sieve[i] && (i & 3) == 3) ++prime_count;

    primes3 = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!primes3) { free(sieve); return; }

    size_t idx = 0;
    for (size_t i = 3; i <= LIMIT; i += 2)
        if (sieve[i] && (i & 3) == 3)
            primes3[idx++] = (uint32_t)i;

    free(sieve);
}

/*======================================================================
 *  Min‑heap for (value, prime_index) pairs.
 *====================================================================*/
typedef struct {
    seq_t  value;
    size_t pidx;   /* index into primes3 */
} Node;

static inline void sift_down(Node *h, size_t n, size_t i)
{
    for (;;) {
        size_t l = 2 * i + 1;
        if (l >= n) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r < n && h[r].value < h[l].value) smallest = r;
        if (h[i].value <= h[smallest].value) break;
        Node tmp = h[i]; h[i] = h[smallest]; h[smallest] = tmp;
        i = smallest;
    }
}

static inline void sift_up(Node *h, size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h[p].value <= h[i].value) break;
        Node tmp = h[p]; h[p] = h[i]; h[i] = tmp;
        i = p;
    }
}

/*======================================================================
 *  Main sequence function.
 *====================================================================*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset < 1 */

    generate_primes3();              /* primes ready */

    const size_t target = (size_t)n;  /* number of terms to generate */

    /* The heap must hold at least all primes plus enough space for new
       elements that are inserted during the process.  This upper bound
       keeps the reallocation logic simple. */
    const size_t heap_cap = prime_count + target;
    Node *heap = (Node *)malloc(heap_cap * sizeof(Node));
    if (!heap) return 0;              /* allocation failure */

    /* Initialize heap with p^2 for every prime p ≡ 3 mod 4. */
    size_t heap_size = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t base = (seq_t)primes3[i] * (seq_t)primes3[i];
        heap[heap_size++] = (Node){ base, i };
    }

    /* Turn array into a min‑heap. */
    for (size_t i = heap_size >> 1; i > 0; --i)
        sift_down(heap, heap_size, i - 1);

    /* Generate output terms until the n‑th one is reached. */
    size_t count = 0;
    while (1) {
        Node top = heap[0];
        ++count;
        if (count == target) {          /* requested term found */
            free(heap);
            return top.value;
        }

        /* Pull the smallest element. */
        heap[0] = heap[--heap_size];
        sift_down(heap, heap_size, 0);

        /* Compute next power for the same prime: multiply by p^2. */
        seq_t  base_sq = (seq_t)primes3[top.pidx] * (seq_t)primes3[top.pidx];
        seq_t  next;

        /* Overflow protection for integral seq_t types (≤ 64 bits). */
        if (sizeof(seq_t) <= 8) {
            seq_t max_val = ~((seq_t)0);                 /* all bits 1 */
            if (top.value > max_val / base_sq) {          /* would overflow */
                /* Skip storing this new value – it is beyond the limit. */
                continue;
            }
            next = top.value * base_sq;                   /* safe multiplication */
        } else {
            /* Arbitrary‑precision build – overflow not a concern. */
            next = top.value * base_sq;
        }

        /* Insert the new element back into the heap. */
        heap[heap_size++] = (Node){ next, top.pidx };
        sift_up(heap, heap_size - 1);
    }
}
