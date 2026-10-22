/*
 * A305312  Discriminant a(n) of the indefinite binary quadratic Markoff form
 *          m(n)*F_{m(n)}(x, y) with m(n) = A002559(n), for n >= 1.
 * https://oeis.org/A305312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*A002559(n)^2 - 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*-----  data structures  -----*/
typedef struct {
    seq_t a, b, c;           /* sorted ascending */
} Triple;

/*-----  dynamic arrays & heap helpers  -----*/
static Triple *visited = NULL;
static size_t visited_len = 0, visited_cap = 0;

static size_t *heap = NULL;
static size_t heap_len = 0, heap_cap = 0;

static seq_t *markoff = NULL;
static size_t markoff_len = 0, markoff_cap = 0;

/* ensure capacity for an array of Triples  */
static void ensure_visited_cap(size_t need)
{
    if (visited_cap < need) {
        size_t newcap = visited_cap ? visited_cap << 1 : 64;
        if (newcap < need) newcap = need;
        Triple *tmp = (Triple *)realloc(visited, newcap * sizeof(Triple));
        visited = tmp;
        visited_cap = newcap;
    }
}

/* ensure capacity for markoff vector  */
static void ensure_markoff_cap(size_t need)
{
    if (markoff_cap < need) {
        size_t newcap = markoff_cap ? markoff_cap << 1 : 64;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(markoff, newcap * sizeof(seq_t));
        markoff = tmp;
        markoff_cap = newcap;
    }
}

/* ensure capacity for heap  */
static void ensure_heap_cap(size_t need)
{
    if (heap_cap < need) {
        size_t newcap = heap_cap ? heap_cap << 1 : 64;
        if (newcap < need) newcap = need;
        size_t *tmp = (size_t *)realloc(heap, newcap * sizeof(size_t));
        heap = tmp;
        heap_cap = newcap;
    }
}

/* heap operations – min‑heap on the 'c' value */
static void heap_push(size_t idx)
{
    ensure_heap_cap(heap_len + 1);
    size_t pos = heap_len++;
    heap[pos] = idx;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (visited[heap[pos]].c < visited[heap[parent]].c) {
            size_t tmp = heap[pos];
            heap[pos] = heap[parent];
            heap[parent] = tmp;
            pos = parent;
        } else break;
    }
}

static size_t heap_pop(void)
{
    size_t ret = heap[0];
    size_t last = --heap_len;
    if (last > 0) {
        heap[0] = heap[last];
        size_t pos = 0;
        while (true) {
            size_t left = (pos << 1) + 1;
            if (left >= heap_len) break;
            size_t right = left + 1;
            size_t child = left;
            if (right < heap_len && visited[heap[right]].c < visited[heap[left]].c)
                child = right;
            if (visited[heap[child]].c < visited[heap[pos]].c) {
                size_t tmp = heap[pos];
                heap[pos] = heap[child];
                heap[child] = tmp;
                pos = child;
            } else break;
        }
    }
    return ret;
}

/* sort a triple so that a <= b <= c  */
static void sort_triple(Triple *t)
{
    if (t->a > t->b) { seq_t tmp = t->a; t->a = t->b; t->b = tmp; }
    if (t->b > t->c) { seq_t tmp = t->b; t->b = t->c; t->c = tmp; }
    if (t->a > t->b) { seq_t tmp = t->a; t->a = t->b; t->b = tmp; }
}

/* check whether a triple already exists in visited  */
static bool visited_contains(const Triple *t, size_t *idx_out)
{
    for (size_t i = 0; i < visited_len; ++i) {
        if (visited[i].a == t->a &&
            visited[i].b == t->b &&
            visited[i].c == t->c) {
            if (idx_out) *idx_out = i;
            return true;
        }
    }
    return false;
}

/*-----  main sequence function  -----*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, return 0 */

    /* initialise data structures on first call */
    if (!visited) {
        visited_cap = 64; visited_len = 0;
        visited = (Triple *)malloc(visited_cap * sizeof(Triple));
        heap_cap = 64; heap_len = 0;
        heap = (size_t *)malloc(heap_cap * sizeof(size_t));
        markoff_cap = 64; markoff_len = 0;
        markoff = (seq_t *)malloc(markoff_cap * sizeof(seq_t));

        /* root triple (1,1,1) */
        Triple root = { (seq_t)1, (seq_t)1, (seq_t)1 };
        ensure_visited_cap(1);
        visited[0] = root;
        visited_len = 1;
        heap_push(0);
    }

    /* generate markoff numbers until we have enough */
    while (markoff_len < (size_t)n) {
        size_t vidx = heap_pop();
        Triple tr = visited[vidx];
        seq_t m = tr.c;   /* largest component */

        /* append to list if not duplicate */
        if (markoff_len == 0 || markoff[markoff_len - 1] != m) {
            ensure_markoff_cap(markoff_len + 1);
            markoff[markoff_len++] = m;
        }

        /* generate three children */
        Triple child;
        seq_t a_ = tr.a, b_ = tr.b, c_ = tr.c;
        /* child 1: replace a */
        child.a = (seq_t)3 * b_ * c_ - a_;
        child.b = b_; child.c = c_;
        sort_triple(&child);
        if (!visited_contains(&child, &vidx)) {
            ensure_visited_cap(visited_len + 1);
            visited[visited_len++] = child;
            heap_push(visited_len - 1);
        }
        /* child 2: replace b */
        child.a = a_; child.b = (seq_t)3 * a_ * c_ - b_; child.c = c_;
        sort_triple(&child);
        if (!visited_contains(&child, &vidx)) {
            ensure_visited_cap(visited_len + 1);
            visited[visited_len++] = child;
            heap_push(visited_len - 1);
        }
        /* child 3: replace c */
        child.a = a_; child.b = b_; child.c = (seq_t)3 * a_ * b_ - c_;
        sort_triple(&child);
        if (!visited_contains(&child, &vidx)) {
            ensure_visited_cap(visited_len + 1);
            visited[visited_len++] = child;
            heap_push(visited_len - 1);
        }
    }

    /* compute result for requested n */
    seq_t m = markoff[n - 1];
    seq_t result = (seq_t)9 * m * m - (seq_t)4;
    return result;
}
