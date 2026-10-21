/*
 * A243722  Number of length n+2 0..3 arrays with no three unequal elements in
 *          a row and no three equal elements in a row and new values 0..3
 *          introduced in 0..3 order.
 * https://oeis.org/A243722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) - 3*a(n-4) (with a(1)=3,
 *            a(2)=6, a(3)=12, a(4)=25)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n<1 */

    /* ------------------------------------------------------------------ */
    /* Cache ---------------------------------------------------------------- */
    /* For C++ builds we use a std::vector to handle arbitrary-precision    */
    /* integer types that need constructors.                                */
    /* For C builds we use a plain malloc/realloc chunk of plain integers.  */
    /* ------------------------------------------------------------------ */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(16);
        cache.push_back((seq_t)3);   // a(1)
        cache.push_back((seq_t)6);   // a(2)
        cache.push_back((seq_t)12);  // a(3)
        cache.push_back((seq_t)25);  // a(4)
    }
    while (cache.size() < (size_t)n) {
        /* compute next value from the recurrence */
        const seq_t a1 = cache[cache.size() - 1];
        const seq_t a2 = cache[cache.size() - 2];
        const seq_t a3 = cache[cache.size() - 3];
        const seq_t a4 = cache[cache.size() - 4];
        const seq_t val = (seq_t)2 * a1 + (seq_t)3 * a2 - (seq_t)4 * a3 - (seq_t)3 * a4;
        cache.push_back(val);
    }
#else
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t len = 0;           /* number of elements already stored */

    /* ensure capacity for at least n elements */
    if (capacity < (size_t)n) {
        size_t newcap = capacity ? capacity * 2 : 16;
        if (newcap < (size_t)n) newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;
        capacity = newcap;
    }

    /* initialise cache with first four terms if empty */
    if (len == 0) {
        cache[0] = (seq_t)3;   // a(1)
        cache[1] = (seq_t)6;   // a(2)
        cache[2] = (seq_t)12;  // a(3)
        cache[3] = (seq_t)25;  // a(4)
        len = 4;
    }

    while (len < (size_t)n) {
        /* compute next value from the recurrence */
        const seq_t a1 = cache[len - 1];
        const seq_t a2 = cache[len - 2];
        const seq_t a3 = cache[len - 3];
        const seq_t a4 = cache[len - 4];
        cache[len] = (seq_t)2 * a1 + (seq_t)3 * a2 - (seq_t)4 * a3 - (seq_t)3 * a4;
        len++;
    }
#endif
    /* ------------------------------------------------------------------ */
    /* Return the requested term */
    /* ------------------------------------------------------------------ */
#ifdef __cplusplus
    return cache[(size_t)n - 1];
#else
    return cache[n - 1];
#endif
}
