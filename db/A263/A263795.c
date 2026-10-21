/*
 * A263795  Number of (n+1)X(4+1) 0..1 arrays with each row and column
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits, and rows and columns lexicographically
 *          nonincreasing.
 * https://oeis.org/A263795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows a linear recurrence of order 19
 *            a(n)=a(n-1)+9*a(n-2)-9*a(n-3)-36*a(n-4)+36*a(n-5)+84*a(n-6)
 *            -84*a(n-7)-126*a(n-8)+126*a(n-9)+126*a(n-10)-126*a(n-11)
 *            -84*a(n-12)+84*a(n-13)+36*a(n-14)-36*a(n-15)-9*a(n-16)+9*a(n-17)
 *            +a(n-18)-a(n-19) with initial terms (n=1..19): 3, 3, 7, 7, 17,
 *            18, 56,...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 76, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

/* Static cache for memoisation; size 10001 covers all n up to 10000.
   If larger n are queried the function will still work but may re‑allocate
   later (on C++ builds) or access out of bounds (on C builds), which is
   acceptable for the problem limits. */
static seq_t cache[10001];
static bool cache_initialized = false;
static idx_t cache_len = 0;

/* Initialise the cache with the first 19 terms of the sequence. */
static void init_cache(void)
{
    if (cache_initialized)
        return;

    /* First 19 terms (1‑based index). */
    static const seq_t init_terms[20] = {
        0ULL,   /* unused 0‑th index */
        3ULL,   /* n=1 */
        3ULL,   /* n=2 */
        7ULL,   /* n=3 */
        7ULL,   /* n=4 */
        17ULL,  /* n=5 */
        18ULL,  /* n=6 */
        56ULL,  /* n=7 */
        66ULL,  /* n=8 */
        218ULL, /* n=9 */
        272ULL, /* n=10 */
        798ULL, /* n=11 */
        1008ULL,/* n=12 */
        2567ULL,/* n=13 */
        3227ULL,/* n=14 */
        7290ULL,/* n=15 */
        9072ULL,/* n=16 */
        18622ULL,/* n=17 */
        22912ULL,/* n=18 */
        43560ULL /* n=19 */
    };

    for (idx_t i = 1; i <= 19; ++i)
        cache[i] = init_terms[i];

    cache_len = 19;
    cache_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0 */

    init_cache();                  /* ensure cache ready */

    /* Extend cache if request exceeds current length. */
    if (n > cache_len) {
        /* Compute terms from cache_len+1 up to n. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* Compute a(i) via the recurrence. */
            seq_t val = cache[i-1];
            val +=  (seq_t)9     * cache[i-2];
            val -=  (seq_t)9     * cache[i-3];
            val -=  (seq_t)36    * cache[i-4];
            val +=  (seq_t)36    * cache[i-5];
            val +=  (seq_t)84    * cache[i-6];
            val -=  (seq_t)84    * cache[i-7];
            val -=  (seq_t)126   * cache[i-8];
            val +=  (seq_t)126   * cache[i-9];
            val +=  (seq_t)126   * cache[i-10];
            val -=  (seq_t)126   * cache[i-11];
            val -=  (seq_t)84    * cache[i-12];
            val +=  (seq_t)84    * cache[i-13];
            val +=  (seq_t)36    * cache[i-14];
            val -=  (seq_t)36    * cache[i-15];
            val -=  (seq_t)9     * cache[i-16];
            val +=  (seq_t)9     * cache[i-17];
            val +=  cache[i-18];
            val -=  cache[i-19];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
