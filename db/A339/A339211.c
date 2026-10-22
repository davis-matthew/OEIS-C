/*
 * A339211  Zeckendorf self numbers: numbers not of the form k + A007895(k).
 * https://oeis.org/A339211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Zeckendorf self numbers: a(n) is the n‑th integer NOT of the form
   k + popcount_Zeck(k), where popcount_Zeck(k) counts the 1‑s in the
   Zeckendorf representation of k.  The function caches state in
   static tables so that calls in arbitrary order up to n ≈ 10 000
   finish comfortably within 2 s. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Global state */
static idx_t *fib = NULL;          /* Fibonacci numbers for Zeckendorf */
static size_t fib_len = 0;          /* number of Fibonacci entries */
static size_t fib_cap = 0;

static uint8_t *generated = NULL;   /* bitmap of k + popcount(k) values */
static size_t gen_cap = 0;          /* size of bitmap array */
static idx_t gen_max = 0;           /* largest k that has been processed */

static seq_t *self_nums = NULL;     /* list of self numbers found so far */
static size_t self_len = 0;         /* number of self numbers stored */
static size_t self_cap = 0;         /* capacity of self_nums array */
static idx_t next_check = 1;        /* integer to test next */

/* Initialise the Fibonacci table so that fib[fib_len-1] > max_val. */
static void init_fib_up_to(idx_t max_val)
{
    if (fib == NULL) {
        fib_cap = 16;
        fib = (idx_t *)malloc(fib_cap * sizeof(idx_t));
        if (!fib) exit(1);
        fib[0] = 1;
        fib[1] = 2;
        fib_len = 2;
    }
    while (fib_len == fib_cap) {
        fib_cap <<= 1;
        fib = (idx_t *)realloc(fib, fib_cap * sizeof(idx_t));
        if (!fib) exit(1);
    }
    while (fib[fib_len-1] <= max_val) {
        fib[fib_len] = fib[fib_len-1] + fib[fib_len-2];
        ++fib_len;
        if (fib_len == fib_cap) {
            fib_cap <<= 1;
            fib = (idx_t *)realloc(fib, fib_cap * sizeof(idx_t));
            if (!fib) exit(1);
        }
    }
}

/* Count the number of Fibonacci summands in the Zeckendorf representation
   of k. */
static size_t zeckendorf_popcount(idx_t k)
{
    size_t cnt = 0;
    idx_t r = k;
    while (r > 0) {
        size_t i;
        for (i = fib_len; i-- > 0; ) {
            if (fib[i] <= r) {
                r -= fib[i];
                ++cnt;
                break;
            }
        }
    }
    return cnt;
}

/* Ensure the generated bitmap has at least 'cap' entries, zero‑padding
   newly added elements. */
static void ensure_generated_cap(idx_t cap)
{
    if (cap <= gen_cap) return;
    size_t new_cap = gen_cap == 0 ? 256 : gen_cap;
    while (new_cap < (size_t)cap) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(generated, new_cap);
    if (!tmp) exit(1);
    for (size_t i = gen_cap; i < new_cap; ++i) tmp[i] = 0;
    generated = tmp;
    gen_cap = new_cap;
}

/* Mark all numbers of the form k + popcount_Zeck(k) for k in
   (old_gen_max, up].  Called with up ≥ gen_max. */
static void generate_up_to(idx_t up)
{
    if (up <= gen_max) return;
    init_fib_up_to(up + 100);          /* more than enough for Zeckendorf */

    for (idx_t k = gen_max + 1; k <= up; ++k) {
        size_t pc = zeckendorf_popcount(k);
        idx_t idx = k + pc;
        if (idx >= (idx_t)gen_cap) ensure_generated_cap(idx + 1);
        generated[idx] = 1;
    }
    gen_max = up;
}

/* Ensure the self_nums array has at least 'cap' entries. */
static void ensure_self_cap(size_t cap)
{
    if (cap <= self_cap) return;
    size_t new_cap = self_cap == 0 ? 128 : self_cap;
    while (new_cap < cap) new_cap += 64;   /* modest growth */
    seq_t *tmp = (seq_t *)realloc(self_nums, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    self_nums = tmp;
    self_cap = new_cap;
}

/* One‑time initialisation of global state.  Called by a(). */
static void init_state(void)
{
    if (self_nums != NULL) return;   /* already initialised */
    self_cap = 128;
    self_nums = (seq_t *)malloc(self_cap * sizeof(seq_t));
    if (!self_nums) exit(1);

    gen_cap = 256;
    generated = (uint8_t *)calloc(gen_cap, 1);
    if (!generated) exit(1);

    fib_cap = 16;
    fib = (idx_t *)malloc(fib_cap * sizeof(idx_t));
    if (!fib) exit(1);
    fib[0] = 1;
    fib[1] = 2;
    fib_len = 2;
}

/* Public entry point – returns the n‑th Zeckendorf self‑number. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, return 0 as safe default */
    init_state();

    while (self_len < (size_t)n) {
        /* Generate candidates up to the next integer we need to test. */
        if (next_check > gen_max) generate_up_to(next_check);

        if (next_check >= (idx_t)gen_cap) ensure_generated_cap(next_check + 1);

        if (!generated[next_check]) {              /* self‑number */
            ensure_self_cap(self_len + 1);
            self_nums[self_len++] = (seq_t)next_check;
        }
        ++next_check;
    }
    return self_nums[n - 1];
}
