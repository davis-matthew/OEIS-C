/*
 * A343065  Side b of integer-sided primitive triangles (a, b, c) whose angle
 *          B = 2*C.
 * https://oeis.org/A343065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A343065: Side b of primitive integer-sided triangles with B = 2*C.
   The formula for a triple (a,b,c) is:
        c = x²
        a = y² - x²   (with x < y < 2x)
        b = x·y
   Primitive triples correspond to coprime pairs (x,y).
   The sequence is produced by sorting all such primitive triples first
   by a and then by b, and taking the b component of the nth element.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct { seq_t a; seq_t b; } triple_t;

static int triple_cmp(const void *p1, const void *p2)
{
    const triple_t *t1 = (const triple_t *)p1;
    const triple_t *t2 = (const triple_t *)p2;
    if (t1->a < t2->a) return -1;
    if (t1->a > t2->a) return 1;
    if (t1->b < t2->b) return -1;
    if (t1->b > t2->b) return 1;
    return 0;
}

/* Integer gcd for idx_t */
static idx_t idx_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static triple_t *triples = NULL;
    static size_t num_triples = 0;
    if (!initialized) {
        /* Pre-generate enough triples.  x <= MAX_X guarantees more than 20000 triples. */
        const idx_t MAX_X = 700;
        size_t capacity = 1024;
        triples = (triple_t *)malloc(capacity * sizeof(triple_t));
        num_triples = 0;
        for (idx_t x = 2; x <= MAX_X; ++x) {
            idx_t x2 = x * x;
            for (idx_t y = x + 1; y < 2 * x; ++y) {
                if (idx_gcd(x, y) != 1) continue;
                /* primitive triple (a,b,c) */
                seq_t a_val = (seq_t)(y * y) - (seq_t)x2;
                seq_t b_val = (seq_t)x * (seq_t)y;
                if (num_triples >= capacity) {
                    capacity *= 2;
                    triples = (triple_t *)realloc(triples, capacity * sizeof(triple_t));
                }
                triples[num_triples].a = a_val;
                triples[num_triples].b = b_val;
                ++num_triples;
            }
        }
        /* sort by a then b */
        qsort(triples, num_triples, sizeof(triple_t), triple_cmp);
        initialized = true;
    }

    if (n == 0 || n > num_triples) /* defensive, not expected for tests */
        return 0;
    return triples[n - 1].b;
}
