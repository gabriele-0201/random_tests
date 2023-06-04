fn id<T>(x: T) -> T {
    x
}

fn composition<A, B, C, F1, F2>(f1: F1, f2: F2) -> impl Fn(A) -> C
where
    F1: Fn(A) -> B,
    F2: Fn(B) -> C,
{
    move |a: A| -> C { f2(f1(a)) }
}

#[derive(Debug, Clone, Copy)]
struct A {
    x: u8,
}

#[derive(Debug)]
struct B {
    x: u8,
}

#[derive(Debug)]
struct C {
    x: u8,
}

fn f1(a: A) -> B {
    B { x: a.x + 2 }
}

fn f2(b: B) -> C {
    C { x: b.x + 2 }
}

fn main() {
    println!("hello world {}", id(5));
    println!("hello world {}", id("prova"));

    let a = A { x: 10 };
    println!("a = {:?}", a);
    println!("f(a) = {:?}", f1(a));
    println!("f2(f1(A)) = {:?}", f2(f1(a)));

    let comp_closure = |a: A| -> C { composition(f1, f2)(a) };
    println!("composition_closure(a)) = {:?}", comp_closure(a));

    // closures have a unique, un-writable type, source: https://doc.rust-lang.org/reference/types/impl-trait.html
    //
    // BUT the same is for every Trait, if we have an 'impl Trait in return position'
    // then is impossible to specify the type
    let comp: _ = composition(f1, f2);
    println!("composition(a)) = {:?}", comp(a));

    let f1_idA = composition(id::<A>, f1);
    let idB_f1 = composition(f1, id::<B>);

    println!(
        "composition with identity, commutativity:  {:?} == {:?}",
        f1_idA(a),
        idB_f1(a)
    );
}
