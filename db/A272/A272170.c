/*
 * A272170  Second most significant bit of Fibonacci numbers > 1 written in
 *          base 2.
 * https://oeis.org/A272170
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(A000045(n)/(2^(ceil(log2(A000045(n) + 1)) - 2))) -
 *            2. * i.e. the second most significant bit of Fibonacci numbers >
 *            1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Big integer (base 2^32) implementation ---------- */

typedef struct {
    uint32_t *w;   /* words, least‑significant first */
    size_t   len;  /* number of used words */
} BigInt;

static void big_init(BigInt *x, size_t len)
{
    x->len = len;
    if (len == 0) {
        x->w = NULL;
    } else {
        x->w = (uint32_t *)malloc(len * sizeof(uint32_t));
    }
}

static void big_destroy(BigInt *x)
{
    free(x->w);
    x->len = 0;
}

static void big_add(const BigInt *a, const BigInt *b, BigInt *out)
{
    size_t maxLen = (a->len > b->len) ? a->len : b->len;
    size_t needed  = maxLen + 1;
    out->w = (uint32_t *)malloc(needed * sizeof(uint32_t));
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < maxLen; ++i) {
        uint64_t sum = carry;
        if (i < a->len) sum += a->w[i];
        if (i < b->len) sum += b->w[i];
        out->w[i] = (uint32_t)(sum & 0xFFFFFFFFu);
        carry     = sum >> 32;
    }
    if (carry) {
        out->w[i] = (uint32_t)carry;
        out->len  = i + 1;
    } else {
        out->len = i;
    }
}

static uint64_t big_bitlen(const BigInt *x)
{
    if (x->len == 0) return 0;
    uint32_t w = x->w[x->len - 1];
    int leading = __builtin_clz(w);           /* 0 <= leading <= 31 */
    return (uint64_t)(x->len - 1) * 32 + (32 - leading);
}

/* Extract `bits` consecutive bits from `x` starting at position `pos` (0 = LSB).  */
static uint32_t big_get_bits(const BigInt *x, size_t pos, size_t bits)
{
    if (bits == 0) return 0;
    size_t wordIdx = pos / 32;
    size_t offset  = pos % 32;
    if (wordIdx >= x->len) return 0;
    if (offset + bits <= 32 || wordIdx + 1 >= x->len) {
        uint32_t v = x->w[wordIdx] >> offset;
        if (bits < 32) v &= ((1u << bits) - 1);
        return v;
    } else {
        uint64_t v = ((uint64_t)x->w[wordIdx] >> offset) |
                     ((uint64_t)x->w[wordIdx + 1] << (32 - offset));
        return (uint32_t)(v & ((1u << bits) - 1));
    }
}

/* ---------- Fibonacci cache -------------------------------- */

typedef struct {
    idx_t  max_n;  /* highest index computed (>=1) */
    size_t cap;    /* allocated capacity */
    BigInt *arr;   /* array of Fibonacci big integers: arr[0] = 0, arr[1] = 1, ... */
} FibCache;

static FibCache fib_cache = {0, 0, NULL};

static void init_cache(void)
{
    if (fib_cache.cap == 0) {
        fib_cache.cap  = 2;
        fib_cache.arr  = (BigInt *)malloc(fib_cache.cap * sizeof(BigInt));
        /* arr[0] = 0 */
        fib_cache.arr[0].len = 0;
        fib_cache.arr[0].w   = NULL;
        /* arr[1] = 1 */
        big_init(&fib_cache.arr[1], 1);
        fib_cache.arr[1].w[0] = 1;
        fib_cache.max_n = 1;
    }
}

/* Ensure Fibonacci numbers up to index n are computed and stored. */
static void ensure_fib(idx_t n)
{
    init_cache();
    if ((idx_t)fib_cache.max_n >= n) return;

    /* Resize array if necessary */
    if ((size_t)n + 1 > fib_cache.cap) {
        size_t newCap = (size_t)n + 1;
        fib_cache.arr = (BigInt *)realloc(fib_cache.arr, newCap * sizeof(BigInt));
        fib_cache.cap = newCap;
    }

    while (fib_cache.max_n < n) {
        BigInt *prev  = &fib_cache.arr[fib_cache.max_n];     /* F(k)   */
        BigInt *prev2 = &fib_cache.arr[fib_cache.max_n - 1]; /* F(k-1) */
        BigInt next;
        big_add(prev, prev2, &next);
        fib_cache.arr[fib_cache.max_n + 1] = next;
        fib_cache.max_n++;
    }
}

/* ---------- Public sequence function -------------------------

   a(n) returns the second most significant bit of Fibonacci number F(n).
   By definition this is 0 or 1.  The function works for any
   index n >= 3; for n < 3 the value is defined as 0.

   The implementation uses an exact big‑integer Fibonacci cache
   so it works correctly for all n, regardless of the size of
   seq_t.

--------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* offset: a(3) is the first term */

    /* Compute Fibonacci numbers up to n */
    ensure_fib(n);

    const BigInt *fn = &fib_cache.arr[n];

    /* Compute bit length of F(n) */
    uint64_t bl = big_bitlen(fn);   /* bl >= 2 for n >= 3 */

    /* Extract the two most significant bits of F(n) */
    size_t shift = (size_t)(bl - 2);              /* where the two bits start */
    uint32_t bits = big_get_bits(fn, shift, 2);   /* value 2 or 3 */

    /* Subtract 2 to obtain 0 or 1 */
    return (seq_t)(bits - 2);
}
